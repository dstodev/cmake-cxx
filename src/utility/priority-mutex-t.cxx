#include "priority-mutex-t.hxx"

// Using triple-mutex based on https://stackoverflow.com/a/11673600 (7/7/2023)

namespace project {

auto priority_mutex_t::lock() -> low_priority_lock_t
{
	return {_access, _next_to_access, _low_priority_access};
}

priority_mutex_t::low_priority_lock_t::low_priority_lock_t(std::mutex& access,
                                                           std::mutex& next_to_access,
                                                           std::mutex& low_priority_access)
    : _access {access, std::defer_lock}
    , _next_to_access {next_to_access, std::defer_lock}
    , _low_priority_access {low_priority_access, std::defer_lock}
{
	_low_priority_access.lock();
	_next_to_access.lock();
	_access.lock();

	_next_to_access.unlock();
}

priority_mutex_t::low_priority_lock_t::~low_priority_lock_t()
{
	_access.unlock();
	_low_priority_access.unlock();
}

auto priority_mutex_t::high_priority_lock() -> high_priority_lock_t
{
	return {_access, _next_to_access};
}

priority_mutex_t::high_priority_lock_t::high_priority_lock_t(std::mutex& access, std::mutex& next_to_access)
    : _access {access, std::defer_lock}
    , _next_to_access {next_to_access, std::defer_lock}
{
	_next_to_access.lock();
	_access.lock();
	_next_to_access.unlock();
}

priority_mutex_t::high_priority_lock_t::~high_priority_lock_t()
{
	_access.unlock();
}

}  // namespace project
