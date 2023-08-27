#include "priority-mutex.hxx"

// Using triple-mutex based on https://stackoverflow.com/a/11673600 (7/7/2023)

namespace project {

PriorityMutex::PriorityMutex(bool default_high_priority)
    : _default_priority(default_high_priority)
{}

void PriorityMutex::lock()
{
	lock(_default_priority);
}

void PriorityMutex::lock(bool high_priority)
{
	if (!high_priority) {
		_low_priority_access.lock();
	}

	_next_to_access.lock();
	_access.lock();
	_next_to_access.unlock();
}

void PriorityMutex::unlock()
{
	unlock(_default_priority);
}

void PriorityMutex::unlock(bool high_priority)
{
	_access.unlock();

	if (!high_priority) {
		_low_priority_access.unlock();
	}
}

}  // namespace project
