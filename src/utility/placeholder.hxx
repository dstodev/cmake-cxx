#ifndef PLACEHOLDER_HXX
#define PLACEHOLDER_HXX

#include <compare>  // Required for operator<=>

class Placeholder
{
public:
	virtual ~Placeholder() = default;

	template <typename... Args>
	explicit constexpr Placeholder([[maybe_unused]] Args&&... args)
	{}

	constexpr Placeholder(Placeholder const& copy) = default;
	constexpr Placeholder(Placeholder&& move) = default;

	Placeholder& operator=(Placeholder const& copy) = default;
	Placeholder& operator=(Placeholder&& move) = default;

	auto operator<=>(Placeholder const& rhs) const = default;

	template <typename T>
	Placeholder const& operator[](T) const
	{
		return *this;
	}

	template <typename... Ts>
	Placeholder const& operator()(Ts...) const
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

#endif  // PLACEHOLDER_HXX
