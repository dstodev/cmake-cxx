#ifndef THREAD_POOL_T_HXX
#define THREAD_POOL_T_HXX

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

namespace project {

template <typename R, typename F, typename... Args>
concept Task = std::is_same_v<R, std::invoke_result_t<F, Args...>>;

template <typename R = void>
class thread_pool_t
{
public:
	using return_type = R;
	using task_type = std::packaged_task<return_type()>;

	explicit thread_pool_t(unsigned int num_threads = 1);

	virtual ~thread_pool_t();
	thread_pool_t(thread_pool_t const& copy) = delete;
	thread_pool_t(thread_pool_t&& move) = default;
	auto operator=(thread_pool_t const& copy) -> thread_pool_t& = delete;
	auto operator=(thread_pool_t&& move) -> thread_pool_t& = default;

	/// Accepts any F(Args...) -> R
	template <typename F, typename... Args>
	    requires Task<R, F, Args...>
	auto add_task(F task, Args&&... args) -> std::future<return_type>;

	void wait();
	void stop();

protected:
	void process_tasks(unsigned int with_thread_index);
	auto pop_task() -> std::unique_ptr<task_type>;
	virtual void task_completed(unsigned int thread_index);

	std::vector<std::thread> _threads;
	std::deque<task_type> _tasks;
	std::atomic_uint _tasks_queued;
	std::mutex _mutex;
	std::condition_variable _task_added;
	std::condition_variable _queue_empty;
	std::atomic_bool _continue;
};

template <typename R>
thread_pool_t<R>::thread_pool_t(unsigned int num_threads)
    : _threads {}
    , _tasks {}
    , _tasks_queued {0}
    , _mutex {}
    , _task_added {}
    , _queue_empty {}
    , _continue {true}
{
	for (unsigned int i {0}; i < num_threads; ++i) {
		_threads.emplace_back([this, i]() { process_tasks(i); });
	}
}

template <typename R>
void thread_pool_t<R>::process_tasks(unsigned int with_thread_index)
{
	while (true) {
		auto task = pop_task();
		if (task) {
			(*task)();
			task_completed(with_thread_index);
			std::this_thread::yield();
		}
		else if (!_continue) {
			// Told to stop with no tasks left
			break;
		}
	}
}

template <typename R>
auto thread_pool_t<R>::pop_task() -> std::unique_ptr<task_type>
{
	auto const stop_waiting = [this]() { return !_continue || !_tasks.empty(); };

	std::unique_ptr<task_type> task;
	{
		std::unique_lock lock(_mutex);
		_task_added.wait(lock, stop_waiting);

		if (!_tasks.empty()) {
			task = std::make_unique<task_type>(std::move(_tasks.front()));
			_tasks.pop_front();
		}
	}

	return task;
}

template <typename R>
void thread_pool_t<R>::task_completed(unsigned int thread_index)
{
	(void) thread_index;  // Unused but useful in derived classes

	_tasks_queued -= 1;

	if (_tasks_queued == 0) {
		_queue_empty.notify_all();
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
    requires Task<R, F, Args...>
auto thread_pool_t<R>::add_task(F task, Args&&... args) -> std::future<return_type>
{
	auto zero_arg_task = std::bind(task, std::forward<Args>(args)...);
	auto async_task = std::packaged_task<return_type()>(std::move(zero_arg_task));
	auto future = async_task.get_future();

	_tasks_queued += 1;

	{
		std::unique_lock lock(_mutex);
		_tasks.emplace_back(std::move(async_task));
	}

	_task_added.notify_one();
	return future;
}

template <typename R>
void thread_pool_t<R>::wait()
{
	std::unique_lock lock(_mutex);
	_queue_empty.wait(lock, [this]() { return _tasks_queued == 0; });
}

template <typename R>
void thread_pool_t<R>::stop()
{
	{
		std::unique_lock lock(_mutex);
		_continue = false;
	}

	// Notify condition variables to wake up & exit
	_task_added.notify_all();

	for (auto& thread : _threads) {
		thread.join();
	}

	_threads.clear();
}

}  // namespace project

#endif  // THREAD_POOL_T_HXX
