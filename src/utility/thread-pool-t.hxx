#ifndef THREAD_POOL_T_HXX
#define THREAD_POOL_T_HXX

#include <barrier>
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

	explicit thread_pool_t(unsigned int num_threads = 1, bool deferred = false);

	virtual ~thread_pool_t();
	thread_pool_t(thread_pool_t const& copy) = delete;
	thread_pool_t(thread_pool_t&& move) = default;
	auto operator=(thread_pool_t const& copy) -> thread_pool_t& = delete;
	auto operator=(thread_pool_t&& move) -> thread_pool_t& = default;

	/// Accepts any F(Args...) -> R
	template <typename F, typename... Args>
	    requires Task<R, F, Args...>
	auto add_task(F task, Args&&... args) -> std::future<return_type>;

	/// If constructed with deferred = true, call start() to begin processing tasks
	void start();

	void wait();
	void stop();

protected:
	enum class state_t
	{
		Deferred,
		Running,
		Stopped
	};

	void process_tasks(unsigned int with_thread_index);
	auto pop_task() -> std::unique_ptr<task_type>;
	virtual void task_completed(unsigned int thread_index);

	state_t _state;

	std::atomic_bool _continue;
	std::vector<std::thread> _threads;
	std::deque<task_type> _tasks;
	std::mutex _mutex;
	std::condition_variable _task_added;
	std::condition_variable _task_completed;
	unsigned int _tasks_queued;
	std::barrier<> _barrier;
};

template <typename R>
thread_pool_t<R>::thread_pool_t(unsigned int num_threads, bool deferred)
    : _state {state_t::Stopped}
    , _continue {true}
    , _threads {}
    , _tasks {}
    , _mutex {}
    , _task_added {}
    , _task_completed {}
    , _tasks_queued {0}
    , _barrier {num_threads + static_cast<unsigned int>(deferred)}
{
	std::barrier barrier(num_threads + static_cast<unsigned int>(deferred));

	for (unsigned int i {0}; i < num_threads; ++i) {
		_threads.emplace_back([this, i]() {
			_barrier.arrive_and_wait();
			process_tasks(i);
		});
	}

	_state = deferred ? state_t::Deferred : state_t::Running;
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

	int remaining;
	{
		// Without locking around _tasks_queued, even if atomic, wait() may freeze
		std::unique_lock<std::mutex> lock(_mutex);
		remaining = --_tasks_queued;
	}

	if (remaining == 0) {
		_task_completed.notify_all();
	}
}

template <typename R>
thread_pool_t<R>::~thread_pool_t()
{
	stop();
}

template <typename R>
template <typename F, typename... Args>
    requires Task<R, F, Args...>
auto thread_pool_t<R>::add_task(F task, Args&&... args) -> std::future<return_type>
{
	auto zero_arg_task = std::bind(task, std::forward<Args>(args)...);
	auto async_task = std::packaged_task<return_type()>(std::move(zero_arg_task));
	auto future = async_task.get_future();

	{
		std::unique_lock lock(_mutex);
		_tasks_queued += 1;
		_tasks.emplace_back(std::move(async_task));
	}

	_task_added.notify_one();
	return future;
}

template <typename R>
void thread_pool_t<R>::start()
{
	if (_state == state_t::Deferred) {
		_barrier.arrive_and_wait();
		_state = state_t::Running;
	}
}

template <typename R>
void thread_pool_t<R>::wait()
{
	std::unique_lock lock(_mutex);
	_task_completed.wait(lock, [this]() { return _tasks_queued == 0; });
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
	_state = state_t::Stopped;
}

}  // namespace project

#endif  // THREAD_POOL_T_HXX
