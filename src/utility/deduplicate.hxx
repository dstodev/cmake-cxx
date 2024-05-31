#ifndef STABLE_DEDUPLICATE_HXX
#define STABLE_DEDUPLICATE_HXX

#include <unordered_set>

namespace project {

/** @brief Remove duplicate values from container while preserving input order.
 */
template <typename T>
T stable_deduplicate(T container)
{
	using element_type = typename T::value_type;
	std::unordered_set<element_type> set;

	// std::remove_if and std::stable_partition are not guaranteed to operate
	// on input elements in order. In the case of std::stable_partition, this
	// may be a bug.

	auto iter {container.begin()};

	for (auto const& element : container) {
		bool const is_unique {set.insert(element).second};

		if (is_unique) {
			*iter++ = element;  // Assign to the next position in the container
		}
	}

	container.erase(iter, container.end());

	return container;
}

}  // namespace project

#endif  // STABLE_DEDUPLICATE_HXX
