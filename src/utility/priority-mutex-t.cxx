#include "priority-mutex-t.hxx"

// Using triple-mutex based on https://stackoverflow.com/a/11673600 (7/7/2023)

namespace project {

priority_mutex_t::priority_mutex_t(bool default_high_priority)
    : _default_priority(default_high_priority)
{}

void priority_mutex_t::lock()
{
	lock(_default_priority);
}

void priority_mutex_t::lock(bool high_priority)
{
	if (!high_priority) {
		_low_priority_access.lock();
	}

	_next_to_access.lock();
	_access.lock();
	_next_to_access.unlock();
}

void priority_mutex_t::unlock()
{
	unlock(_default_priority);
}

void priority_mutex_t::unlock(bool high_priority)
{
	_access.unlock();

	if (!high_priority) {
		_low_priority_access.unlock();
	}
}

}  // namespace project
