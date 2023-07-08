#include "state.hxx"

#include <mutex>

namespace {
// Using triple-mutex to give priority to the reader thread.
// Based on https://stackoverflow.com/a/11673600 (7/7/2023)
std::mutex access;
std::mutex next_to_access;
std::mutex low_priority_access;
project::input::State VIRTUAL;
std::unique_lock<std::mutex> lock_lpa {low_priority_access, std::defer_lock};
std::unique_lock<std::mutex> lock_nta {next_to_access, std::defer_lock};
std::unique_lock<std::mutex> lock_a {access, std::defer_lock};
}  // namespace

namespace project::input {

State clone()
{
	// Acquire lock with high-priority
	lock_nta.lock();
	lock_a.lock();
	lock_nta.unlock();

	State state = VIRTUAL;

	lock_a.unlock();

	return state;
}

void update(State const& input)
{
	// Acquire lock with low-priority
	lock_lpa.lock();

	lock_nta.lock();
	lock_a.lock();
	lock_nta.unlock();

	VIRTUAL = input;

	lock_a.unlock();

	lock_lpa.unlock();
}

}  // namespace project::input
