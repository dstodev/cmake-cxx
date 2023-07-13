#ifndef PLACEHOLDER_T_HXX
#define PLACEHOLDER_T_HXX

#include <compare>  // Required for operator<=>

class placeholder_t
{
public:
	virtual ~placeholder_t() = default;

	template <typename... Args>
	explicit constexpr placeholder_t([[maybe_unused]] Args&&... args)
	{}

	constexpr placeholder_t(placeholder_t const& copy) = default;
	constexpr placeholder_t(placeholder_t&& move) = default;

	placeholder_t& operator=(placeholder_t const& copy) = default;
	placeholder_t& operator=(placeholder_t&& move) = default;

	auto operator<=>(placeholder_t const& rhs) const = default;

	template <typename T>
	placeholder_t const& operator[](T) const
	{
		return *this;
	}

	template <typename... Ts>
	placeholder_t const& operator()(Ts...) const
	{
		return *this;
	}

#define generic_operator(symbol) \
	template <typename... Args> \
	auto& operator symbol(Args...) \
	{ \
		return *this; \
	}

	// These implementations allow usage in common expressions.
	generic_operator(+);
	generic_operator(+=);
	generic_operator(-);
	generic_operator(-=);
	generic_operator(*);
	generic_operator(*=);
	generic_operator(/);
	generic_operator(/=);
	generic_operator(%);
	generic_operator(%=);
};

#endif  // PLACEHOLDER_T_HXX
