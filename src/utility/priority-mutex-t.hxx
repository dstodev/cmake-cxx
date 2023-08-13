#ifndef PRIORITY_MUTEX_T_HXX
#define PRIORITY_MUTEX_T_HXX

#include <mutex>

#include <dll-export.h>

namespace project {

class DLL priority_mutex_t
{
public:
	priority_mutex_t() = default;

	class low_priority_lock_t;
	class high_priority_lock_t;

	virtual ~priority_mutex_t() = default;
	priority_mutex_t(priority_mutex_t const& copy) = delete;
	priority_mutex_t(priority_mutex_t&& move) = delete;
	auto operator=(priority_mutex_t const& copy) -> priority_mutex_t& = delete;
	auto operator=(priority_mutex_t&& move) -> priority_mutex_t& = delete;

	[[nodiscard]]
	auto lock() -> low_priority_lock_t;

	[[nodiscard]]
	auto high_priority_lock() -> high_priority_lock_t;

protected:
	std::mutex _access;
	std::mutex _next_to_access;
	std::mutex _low_priority_access;
};

class DLL priority_mutex_t::low_priority_lock_t
{
public:
	low_priority_lock_t(std::mutex& access, std::mutex& next_to_access, std::mutex& low_priority_access);

	~low_priority_lock_t();

private:
	std::unique_lock<std::mutex> _access;
	std::unique_lock<std::mutex> _next_to_access;
	std::unique_lock<std::mutex> _low_priority_access;
};

class DLL priority_mutex_t::high_priority_lock_t
{
public:
	high_priority_lock_t(std::mutex& access, std::mutex& next_to_access);

	~high_priority_lock_t();

private:
	std::unique_lock<std::mutex> _access;
	std::unique_lock<std::mutex> _next_to_access;
};

}  // namespace project

#endif  // PRIORITY_MUTEX_T_HXX
