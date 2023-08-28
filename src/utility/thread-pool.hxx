#ifndef THREAD_POOL_HXX
#define THREAD_POOL_HXX

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <latch>
#include <mutex>
#include <thread>
#include <type_traits>
#include <vector>

namespace project {

template <typename R, typename F, typename... Args>
concept Task = std::is_same_v<R, std::invoke_result_t<F, Args...>>;

template <typename R = void>
class ThreadPool
{
public:
	using return_type = R;
	using task_type = std::packaged_task<return_type()>;

	/** @brief Construct a thread pool.
	    @param num_threads Number of threads executing tasks
	    @param deferred If true, threads will not start until start() is called
	 */
	explicit ThreadPool(unsigned int num_threads = 1, bool deferred = false);

	virtual ~ThreadPool();
	ThreadPool(ThreadPool const& copy) = delete;
	ThreadPool(ThreadPool&& move) = default;
	auto operator=(ThreadPool const& copy) -> ThreadPool& = delete;
	auto operator=(ThreadPool&& move) -> ThreadPool& = default;

	/** @brief Add a task to the queue.
	    @param task Task to add
	    @param args Arguments to pass to task
	    @return Future to task result

	    Accepts any function to eventually execute. When it does, the returned
	    future will contain the result of the function.

	    Accepts any F(Args...) -> R
	 */
	template <typename F, typename... Args>
	    requires Task<R, F, Args...>
	auto add_task(F task, Args&&... args) -> std::future<return_type>;

	/// If constructed with deferred = true, call start() to begin processing tasks.
	void start();

	/// Wait for all added tasks to complete.
	void wait();

	/// Wait for all added tasks to complete then stop all threads.
	void stop();

	/// @return True if threads are running, false otherwise.
	bool is_running() const;

protected:
	enum class State
	{
		Deferred,
		Running,
		Stopped
	};

	void process_tasks(unsigned int with_thread_index);
	auto pop_task() -> std::unique_ptr<task_type>;
	virtual void task_completed(unsigned int thread_index);

	State _state;

	std::atomic_bool _continue;
	std::vector<std::thread> _threads;
	std::deque<task_type> _task_queue;
	/// Incremented before added to queue & decremented after task is executed
	unsigned int _task_count;
	std::mutex _task_queue_mutex;
	std::mutex _task_count_mutex;
	std::condition_variable _task_added;
	std::condition_variable _task_completed;
	std::latch _latch;
};

template <typename R>
ThreadPool<R>::ThreadPool(unsigned int num_threads, bool deferred)
    : _state {State::Stopped}
    , _continue {true}
    , _threads {}
    , _task_queue {}
    , _task_count {0}
    , _task_queue_mutex {}
    , _task_count_mutex {}
    , _task_added {}
    , _task_completed {}
    , _latch {num_threads + static_cast<unsigned int>(deferred)}
{
	std::latch latch(num_threads + static_cast<unsigned int>(deferred));

	for (unsigned int i {0}; i < num_threads; ++i) {
		_threads.emplace_back([this, i]() {
			_latch.arrive_and_wait();
			process_tasks(i);
		});
	}

	_state = deferred ? State::Deferred : State::Running;
}

template <typename R>
void ThreadPool<R>::process_tasks(unsigned int with_thread_index)
{
	while (true) {
		auto task = pop_task();
		if (task) {
			(*task)();
			task_completed(with_thread_index);
			std::this_thread::yield();
		}
		else if (!_continue) {
			// Told to stop with no tasks left.
			break;
		}
	}
}

template <typename R>
auto ThreadPool<R>::pop_task() -> std::unique_ptr<task_type>
{
	auto const stop_waiting = [this]() { return !_continue || !_task_queue.empty(); };

	std::unique_ptr<task_type> task;
	{
		std::unique_lock lock(_task_queue_mutex);
		_task_added.wait(lock, stop_waiting);

		if (!_task_queue.empty()) {
			task = std::make_unique<task_type>(std::move(_task_queue.front()));
			_task_queue.pop_front();
		}
	}

	return task;
}

template <typename R>
void ThreadPool<R>::task_completed(unsigned int thread_index)
{
	(void) thread_index;  // Unused but useful in derived classes

	int remaining;
	{
		// Without locking around _task_count_mutex, even if atomic, wait() may freeze.
		// This may be because the value could change even when the wait() predicate
		// has the mutex lock.
		std::lock_guard<std::mutex> lock(_task_count_mutex);
		remaining = --_task_count;
	}

	if (remaining == 0) {
		_task_completed.notify_all();
	}
}

template <typename R>
ThreadPool<R>::~ThreadPool()
{
	stop();
}

template <typename R>
template <typename F, typename... Args>
    requires Task<R, F, Args...>
auto ThreadPool<R>::add_task(F task, Args&&... args) -> std::future<return_type>
{
	auto zero_arg_task = std::bind(task, std::forward<Args>(args)...);
	auto async_task = std::packaged_task<return_type()>(std::move(zero_arg_task));
	auto future = async_task.get_future();

	{
		std::lock_guard lock(_task_count_mutex);
		_task_count += 1;
	}
	{
		std::lock_guard lock(_task_queue_mutex);
		_task_queue.emplace_back(std::move(async_task));
	}

	_task_added.notify_one();
	return future;
}

template <typename R>
void ThreadPool<R>::start()
{
	if (_state == State::Deferred) {
		_latch.arrive_and_wait();
		_state = State::Running;
	}
}

template <typename R>
void ThreadPool<R>::wait()
{
	start();  // In case constructed with deferred=true but not yet started

	std::unique_lock lock(_task_count_mutex);
	_task_completed.wait(lock, [this]() { return _task_count == 0; });
}

template <typename R>
void ThreadPool<R>::stop()
{
	start();  // In case constructed with deferred=true but not yet started

	{
		std::lock_guard lock(_task_queue_mutex);
		_continue = false;
	}

	// Notify condition variables to wake up & exit
	_task_added.notify_all();
	/* _task_completed.notify_all();  Let threads wait for empty queue */

	for (auto& thread : _threads) {
		thread.join();
	}

	_threads.clear();
	_state = State::Stopped;
}

template <typename R>
bool ThreadPool<R>::is_running() const
{
	return _state == State::Running;
}

}  // namespace project

#endif  // THREAD_POOL_HXX
