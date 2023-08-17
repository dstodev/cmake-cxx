#ifndef PRIORITY_MUTEX_HXX
#define PRIORITY_MUTEX_HXX

#include <mutex>

#include <dll-export.h>

namespace project {

// Satisfies BasicLockable
// https://en.cppreference.com/w/cpp/named_req/BasicLockable

class DLL PriorityMutex
{
public:
	explicit PriorityMutex(bool default_high_priority = false);

	void lock();
	void lock(bool high_priority);

	void unlock();
	void unlock(bool high_priority);

protected:
	std::mutex _access;
	std::mutex _next_to_access;
	std::mutex _low_priority_access;

	bool _default_priority;
};

}  // namespace project

#endif  // PRIORITY_MUTEX_HXX
