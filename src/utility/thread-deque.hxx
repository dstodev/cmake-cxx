#ifndef THREAD_DEQUE_HXX
#define THREAD_DEQUE_HXX

#include <deque>
#include <mutex>
#include <utility>

namespace project {

/** @brief Exposes some of the same behavior as std::deque with thread-safe operations.
    @tparam T Element type

    See std::deque documentation for details: https://en.cppreference.com/w/cpp/container/deque
 */
template <typename T>
class ThreadDeque
{
public:
	using value_type = T;
	using container_type = std::deque<T>;
	using size_type = typename container_type::size_type;

	bool empty() const;
	auto size() const -> size_type;

	template <typename... Args>
	void emplace_front(Args&&... args);
	void push_front(value_type const& value);
	void push_front(value_type&& value);
	/** @brief Remove and return an element from the front of the deque.
	    @return Popped value

	    If empty, blocks until an element is available.
	    This function differs from the std::deque interface by returning the popped value.
	    This is so threads can conveniently pop values in a thread-safe manner.
	 */
	auto pop_front() -> value_type;

	template <typename... Args>
	void emplace_back(Args&&... args);
	void push_back(value_type const& value);
	void push_back(value_type&& value);
	/** @brief Remove and return an element from the back of the deque.
	    @return Popped value

	    If empty, blocks until an element is available.
	    This function differs from the std::deque interface by returning the popped value.
	    This is so threads can conveniently pop values in a thread-safe manner.
	 */
	auto pop_back() -> value_type;

protected:
	/* Because front() and back() return references, they probably do not function
	   as expected between multiple threads. They are useful for testing, so protect them.
	 */
	auto front() const -> decltype(std::declval<container_type const>().front());
	auto back() const -> decltype(std::declval<container_type const>().back());

	container_type _data;
	mutable std::mutex _mutex;
	std::condition_variable _element_added;
};

template <typename T>
bool ThreadDeque<T>::empty() const
{
	return _data.empty();
}

template <typename T>
auto ThreadDeque<T>::size() const -> size_type
{
	return _data.size();
}

template <typename T>
auto ThreadDeque<T>::front() const -> decltype(std::declval<container_type const>().front())
{
	return _data.front();
}

template <typename T>
template <typename... Args>
void ThreadDeque<T>::emplace_front(Args&&... args)
{
	{
		std::lock_guard lock(_mutex);
		_data.emplace_front(std::forward<Args>(args)...);
	}
	_element_added.notify_one();
}

template <typename T>
void ThreadDeque<T>::push_front(value_type const& value)
{
	{
		std::lock_guard lock(_mutex);
		_data.push_front(value);
	}
	_element_added.notify_one();
}

template <typename T>
void ThreadDeque<T>::push_front(value_type&& value)
{
	{
		std::lock_guard lock(_mutex);
		_data.push_front(std::move(value));
	}
	_element_added.notify_one();
}

template <typename T>
auto ThreadDeque<T>::pop_front() -> value_type
{
	value_type value;
	{
		std::unique_lock lock(_mutex);
		_element_added.wait(lock, [this]() { return !_data.empty(); });

		value = std::move(_data.front());
		_data.pop_front();
	}
	return value;
}

template <typename T>
auto ThreadDeque<T>::back() const -> decltype(std::declval<container_type const>().back())
{
	return _data.back();
}

template <typename T>
template <typename... Args>
void ThreadDeque<T>::emplace_back(Args&&... args)
{
	{
		std::lock_guard lock(_mutex);
		_data.emplace_back(std::forward<Args>(args)...);
	}
	_element_added.notify_one();
}

template <typename T>
void ThreadDeque<T>::push_back(value_type const& value)
{
	{
		std::lock_guard lock(_mutex);
		_data.push_back(value);
	}
	_element_added.notify_one();
}

template <typename T>
void ThreadDeque<T>::push_back(value_type&& value)
{
	{
		std::lock_guard lock(_mutex);
		_data.push_back(std::move(value));
	}
	_element_added.notify_one();
}

template <typename T>
auto ThreadDeque<T>::pop_back() -> value_type
{
	value_type value;
	{
		std::unique_lock lock(_mutex);
		_element_added.wait(lock, [this]() { return !_data.empty(); });

		value = std::move(_data.back());
		_data.pop_back();
	}
	return value;
}

}  // namespace project

#endif  // THREAD_DEQUE_HXX
