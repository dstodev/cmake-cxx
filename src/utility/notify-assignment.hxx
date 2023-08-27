#ifndef NOTIFY_ASSIGNMENT_HXX
#define NOTIFY_ASSIGNMENT_HXX

#include <algorithm>
#include <functional>
#include <memory>
#include <type_traits>

namespace project {

/** @brief Owns a value and notifies observers when the value changes.
    @tparam T
 */
template <typename T>
class NotifyAssignment
{
public:
	using value_type = T;
	using callback_type = std::function<void(value_type const& old_value, value_type const& new_value)>;

	template <typename... Args>
	explicit NotifyAssignment(Args&&... args);

	void add_observer(callback_type callback);

	operator value_type const&() const;

	template <typename U = value_type>
	auto operator=(value_type&& value) -> std::enable_if_t<std::is_move_assignable_v<U>, NotifyAssignment&>;

	template <typename U = value_type>
	auto operator=(value_type const& value) -> std::enable_if_t<std::is_copy_assignable_v<U>, NotifyAssignment&>;

private:
	std::unique_ptr<value_type> _value;
	std::vector<callback_type> _observers;

	void notify(value_type const& old_value, value_type const& new_value);
};

template <typename T>
template <typename... Args>
NotifyAssignment<T>::NotifyAssignment(Args&&... args)
    : _value(std::make_unique<value_type>(std::forward<Args>(args)...))
{}

template <typename T>
void NotifyAssignment<T>::add_observer(callback_type callback)
{
	_observers.emplace_back(std::move(callback));
}

template <typename T>
NotifyAssignment<T>::operator value_type const&() const
{
	return *_value;
}

template <typename T>
template <typename U>
auto NotifyAssignment<T>::operator=(value_type&& value)
    -> std::enable_if_t<std::is_move_assignable_v<U>, NotifyAssignment&>
{
	auto old_value = std::move(*_value);
	*_value = std::move(value);
	notify(old_value, *_value);
	return *this;
}

template <typename T>
template <typename U>
auto NotifyAssignment<T>::operator=(value_type const& value)
    -> std::enable_if_t<std::is_copy_assignable_v<U>, NotifyAssignment&>
{
	auto old_value = *_value;
	*_value = value;
	notify(old_value, *_value);
	return *this;
}

template <typename T>
void NotifyAssignment<T>::notify(value_type const& old_value, value_type const& new_value)
{
	std::for_each(_observers.begin(), _observers.end(), [&](auto const& observer) { observer(old_value, new_value); });
}

}  // namespace project

#endif  // NOTIFY_ASSIGNMENT_HXX
