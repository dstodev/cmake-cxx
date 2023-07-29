#ifndef NOTIFY_ASSIGNMENT_HXX
#define NOTIFY_ASSIGNMENT_HXX

#include <functional>
#include <memory>
#include <type_traits>

namespace project {

/**
 * @brief Owns a value and notifies observers when the value changes.
 * @tparam T
 */
template <typename T>
class NotifyAssignment
{
public:
	using value_type = T;
	using callback_type = std::function<void(value_type const& old_value, value_type const& new_value)>;

	template <typename... Args>
	explicit NotifyAssignment(Args&&... args)
	    : _value(std::make_unique<value_type>(std::forward<Args>(args)...))
	{}

	void add_observer(callback_type callback)
	{
		_observers.emplace_back(std::move(callback));
	}

	operator value_type const&() const
	{
		return *_value;
	}

	template <typename U = value_type, std::enable_if_t<std::is_move_assignable_v<U>, int> = 0>
	auto operator=(value_type&& value) -> NotifyAssignment&
	{
		auto old_value = std::move(*_value);
		*_value = std::move(value);
		notify(old_value, *_value);
		return *this;
	}

	template <typename U = value_type, std::enable_if_t<std::is_copy_assignable_v<U>, int> = 0>
	auto operator=(value_type const& value) -> NotifyAssignment&
	{
		auto old_value = *_value;
		*_value = value;
		notify(old_value, *_value);
		return *this;
	}

private:
	void notify(value_type const& old_value, value_type const& new_value)
	{
		for (auto const& observer : _observers) {
			observer(old_value, new_value);
		}
	}

	std::unique_ptr<value_type> _value;
	std::vector<callback_type> _observers;
};

}  // namespace project

#endif  // NOTIFY_ASSIGNMENT_HXX
