#ifndef THREAD_POOL_T_HXX
#define THREAD_POOL_T_HXX

#include <condition_variable>
#include <deque>
#include <functional>
#include <future>
#include <thread>
#include <type_traits>
#include <vector>

#include <priority-mutex-t.hxx>

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
	auto add_task(F task, Args&&... args) -> std::shared_future<return_type>;

	void wait();
	void stop();

	// TODO: Only do this in a child class for testing?
	auto thread_task_contributions() const -> std::vector<unsigned int> const&;

protected:
	void process_tasks(unsigned int with_id);
	auto pop_task() -> std::unique_ptr<task_type>;

	std::vector<std::thread> _threads;
	std::deque<task_type> _tasks;
	std::vector<std::shared_future<return_type>> _futures;
	priority_mutex_t _mutex;
	std::condition_variable_any _task_added;
	std::atomic_bool _continue;

	std::atomic_uint _next_id;
	std::vector<unsigned int> _thread_contributions;
};

template <typename R>
thread_pool_t<R>::thread_pool_t(unsigned int num_threads)
    : _threads {}
    , _tasks {}
    , _futures {}
    , _mutex {}
    , _task_added {}
    , _continue {true}
    , _next_id {0}
    , _thread_contributions(num_threads, 0)
{
	for (unsigned int i {0}; i < num_threads; ++i) {
		_threads.emplace_back([this]() {
			auto const with_id = _next_id++;
			process_tasks(with_id);
		});
	}
}

template <typename R>
void thread_pool_t<R>::process_tasks(unsigned int with_id)
{
	while (true) {
		auto task = pop_task();
		if (task) {
			(*task)();
			_thread_contributions[with_id] += 1;
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
	auto const has_tasks = [this]() { return !_tasks.empty(); };
	auto const stop_waiting = [this, has_tasks]() { return !_continue || has_tasks(); };

	std::unique_ptr<task_type> task;
	{
		std::unique_lock lock(_mutex);
		_task_added.wait(lock, stop_waiting);

		if (has_tasks()) {
			task = std::make_unique<task_type>(std::move(_tasks.front()));
			_tasks.pop_front();
		}
	}
	return task;
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
auto thread_pool_t<R>::add_task(F task, Args&&... args) -> std::shared_future<return_type>
{
	auto zero_arg_task = std::bind(task, std::forward<Args>(args)...);
	auto async_task = std::packaged_task<return_type()>(std::move(zero_arg_task));
	auto future = std::shared_future(async_task.get_future());

	_mutex.lock();
	_futures.push_back(future);
	_tasks.emplace_back(std::move(async_task));
	_mutex.unlock();

	_task_added.notify_one();
	return future;
}

template <typename R>
void thread_pool_t<R>::wait()
{
	for (auto& future : _futures) {
		future.wait();
	}
	_futures.clear();
}

template <typename R>
void thread_pool_t<R>::stop()
{
	_mutex.lock(true);
	_continue = false;
	_mutex.unlock(true);

	// Notify all condition variables to wake up & exit
	_task_added.notify_all();

	for (auto& thread : _threads) {
		thread.join();
	}
	_threads.clear();
	_futures.clear();
}

template <typename R>
auto thread_pool_t<R>::thread_task_contributions() const -> std::vector<unsigned int> const&
{
	return _thread_contributions;
}

}  // namespace project

#endif  // THREAD_POOL_T_HXX
