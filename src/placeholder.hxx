#ifndef PLACEHOLDER_HXX
#define PLACEHOLDER_HXX

#include <compare>

class placeholder_t
{
public:
	template <typename... Args>
	constexpr explicit placeholder_t([[maybe_unused]] Args&&... args) noexcept
	{}
	virtual ~placeholder_t() = default;

	constexpr placeholder_t(placeholder_t const& copy) noexcept = default;
	constexpr placeholder_t(placeholder_t&& move) noexcept = default;

	placeholder_t& operator=(placeholder_t const& copy) noexcept = default;
	placeholder_t& operator=(placeholder_t&& move) noexcept = default;

	auto operator<=>(placeholder_t const& rhs) const = default;

	template <typename T>
	placeholder_t const& operator[](T _) const
	{
		return *this;
	}

	template <typename... Ts>
	placeholder_t const& operator()(Ts... _) const
	{
		return *this;
	}

#define generic_operator(symbol) \
	template <typename... Args> \
	[[maybe_unused]] auto& operator symbol([[maybe_unused]] Args... args) \
	{ \
		return *this; \
	}

	generic_operator(+);  // Semicolons for correct auto-format
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
