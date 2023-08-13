#ifndef PRIORITY_MUTEX_T_HXX
#define PRIORITY_MUTEX_T_HXX

#include <atomic>
#include <mutex>

#include <dll-export.h>

namespace project {

class DLL priority_mutex_t
{
public:
	priority_mutex_t(bool default_high_priority = false);

	void lock();
	void lock(bool high_priority);

	void unlock();
	void unlock(bool high_priority);

protected:
	std::mutex _access;
	std::mutex _next_to_access;
	std::mutex _low_priority_access;

	std::atomic_bool _default_priority;
};

}  // namespace project

#endif  // PRIORITY_MUTEX_T_HXX
