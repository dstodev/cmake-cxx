#ifndef THREAD_POOL_T_HXX
#define THREAD_POOL_T_HXX

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
	thread_pool_t(thread_pool_t const&) = delete;
	thread_pool_t(thread_pool_t&&) = default;
	auto operator=(thread_pool_t const&) -> thread_pool_t& = delete;
	auto operator=(thread_pool_t&&) -> thread_pool_t& = default;

	template <typename F, typename... Args>
	auto add_task(F task, Args&&... args) -> std::future<return_type>;

	void stop();

	auto thread_task_contributions() const -> std::vector<int> const&;

private:
	void process_tasks();

	std::vector<std::thread> _threads;
	std::deque<task_type> _tasks;
	std::mutex _mutex;
	std::condition_variable _condition;
	std::atomic_bool _continue;

	std::atomic_int _last_id;
	std::vector<int> _thread_contributions;
};

template <typename R>
thread_pool_t<R>::thread_pool_t(int num_threads)
    : _threads {}
    , _tasks {}
    , _mutex {}
    , _condition {}
    , _continue {true}
    , _last_id {0}
    , _thread_contributions(num_threads, 0)
{
	for (int i {0}; i < num_threads; ++i) {
		_threads.emplace_back([this]() { process_tasks(); });
	}
}

template <typename R>
void thread_pool_t<R>::process_tasks()
{
	auto const has_tasks = [this]() { return !_tasks.empty(); };
	auto const stop_waiting = [this, has_tasks]() { return !_continue || has_tasks(); };

	int id = ++_last_id;

	while (true) {
		std::unique_ptr<task_type> task;
		{
			std::unique_lock<std::mutex> lock {_mutex};

			_condition.wait(lock, stop_waiting);

			if (has_tasks()) {
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
			_thread_contributions[id - 1] += 1;
			std::this_thread::yield();
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

template <typename R>
auto thread_pool_t<R>::thread_task_contributions() const -> std::vector<int> const&
{
	return _thread_contributions;
}

}  // namespace project

#endif  // THREAD_POOL_T_HXX
