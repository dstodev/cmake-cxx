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
	using task_type = std::packaged_task<R()>;  // https://en.cppreference.com/w/cpp/thread/packaged_task

	explicit thread_pool_t(int num_threads = 1)
	    : _tasks {}
	    , _mutex {}
	    , _condition {}
	    , _continue {true}
	{
		for (int i {0}; i < num_threads; ++i) {
			_threads.emplace_back([this]() {  // https://en.cppreference.com/w/cpp/thread/thread/thread
				while (_continue) {
					task_type task;
					{
						std::unique_lock<std::mutex> lock {_mutex};
						_condition.wait(lock, [this]() { return !_continue || !_tasks.empty(); });

						if (!_tasks.empty()) {
							task = std::move(_tasks.front());
							_tasks.pop_front();
						}
						else if (!_continue) {
							// Told to stop & nothing left to do.
							break;
						}
						// else, spurious wakeup
					}
					task();
				}
			});
		}
	}

	~thread_pool_t()
	{
		stop();
	}

	template <typename F, typename... Args>
	auto add_task(F task, Args&&... args) -> std::future<return_type>
	{
		auto pack = task_type(std::bind(task, std::forward<Args>(args)...));
		auto future = pack.get_future();
		{
			std::unique_lock<std::mutex> lock {_mutex};
			_tasks.push_back(std::move(pack));
		}
		_condition.notify_one();
		return future;
	}

	void stop()
	{
		_continue = false;
		_condition.notify_all();
		for (auto& thread : _threads) {
			thread.join();
		}
	}

private:
	std::vector<std::thread> _threads;  // https://en.cppreference.com/w/cpp/container/vector
	std::deque<task_type> _tasks;  // https://en.cppreference.com/w/cpp/container/deque
	std::mutex _mutex;  // https://en.cppreference.com/w/cpp/thread/mutex
	std::condition_variable _condition;  // https://en.cppreference.com/w/cpp/thread/condition_variable
	std::atomic_bool _continue;
};

}  // namespace project

#endif  // THREAD_POOL_T_HXX
