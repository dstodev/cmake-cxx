#ifndef THREAD_DEQUE_HXX
#define THREAD_DEQUE_HXX

#include <deque>
#include <mutex>
#include <utility>

namespace project {

// https://en.cppreference.com/w/cpp/container/deque

/** @brief Extends std::deque with thread-safe operations.
    @tparam T Element type
 */
template <typename T>
class ThreadDeque
{
public:
	using value_type = T;
	using container_type = std::deque<T>;
	using size_type = typename container_type::size_type;

	ThreadDeque();

	bool empty() const;
	auto size() const -> size_type;

	auto front() const -> decltype(std::declval<container_type const>().front());
	template <typename... Args>
	void emplace_front(Args&&... args);
	void push_front(value_type const& value);
	void push_front(value_type&& value);
	auto pop_front() -> value_type;

	auto back() const -> decltype(std::declval<container_type const>().back());
	template <typename... Args>
	void emplace_back(Args&&... args);
	void push_back(value_type const& value);
	void push_back(value_type&& value);
	auto pop_back() -> value_type;

protected:
	container_type _data;
	mutable std::mutex _mutex;
	std::condition_variable _element_added;
};

template <typename T>
ThreadDeque<T>::ThreadDeque()
    : _data()
    , _mutex()
    , _element_added()
{}

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
