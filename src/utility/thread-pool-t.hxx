#ifndef THREAD_POOL_T_HXX
#define THREAD_POOL_T_HXX

#include <barrier>
#include <deque>
#include <functional>
#include <future>
#include <thread>
#include <vector>

namespace project {

template <typename R = void>
class thread_pool_t
{
public:
	using return_type = R;
	using task_type = std::packaged_task<return_type()>;

	explicit thread_pool_t(int num_threads = 1);
	~thread_pool_t();

	template <typename F, typename... Args>
	auto add_task(F task, Args&&... args) -> std::future<return_type>;

	void stop();

private:
	void process_tasks();

	std::vector<std::thread> _threads;
	std::deque<task_type> _tasks;
	std::mutex _mutex;
	std::condition_variable _condition;
	std::atomic_bool _continue;
};

template <typename R>
thread_pool_t<R>::thread_pool_t(int num_threads)
    : _tasks {}
    , _mutex {}
    , _condition {}
    , _continue {true}
{
	for (int i {0}; i < num_threads; ++i) {
		_threads.emplace_back([this]() { process_tasks(); });
	}
}

template <typename R>
void thread_pool_t<R>::process_tasks()
{
	while (true) {
		std::unique_ptr<task_type> task;
		{
			std::unique_lock<std::mutex> lock {_mutex};

			_condition.wait(lock, [this]() {
				return !(_continue && _tasks.empty());
			});  // if continue && queue-is-empty, keep waiting

			if (!_tasks.empty()) {
				task = std::make_unique<task_type>(std::move(_tasks.front()));
				_tasks.pop_front();
			}
			else if (!_continue) {
				// Told to stop and nothing left to do.
				break;
			}
		}
		if (task) {
			(*task)();
		}
	}
}

template <typename R>
thread_pool_t<R>::~thread_pool_t()
{
	if (_continue) {
		stop();
	}
}

template <typename R>
template <typename F, typename... Args>
auto thread_pool_t<R>::add_task(F task, Args&&... args) -> std::future<return_type>
{
	auto zero_arg_task = std::bind(task, std::forward<Args>(args)...);
	auto async_task = std::packaged_task<return_type()>(std::move(zero_arg_task));
	auto future = async_task.get_future();
	{
		std::unique_lock<std::mutex> lock {_mutex};
		_tasks.emplace_back(std::move(async_task));
	}
	_condition.notify_one();
	return future;
}

template <typename R>
void thread_pool_t<R>::stop()
{
	{
		std::unique_lock<std::mutex> lock {_mutex};
		_continue = false;
	}
	_condition.notify_all();

	for (auto& thread : _threads) {
		thread.join();
	}
}

}  // namespace project

#endif  // THREAD_POOL_T_HXX
