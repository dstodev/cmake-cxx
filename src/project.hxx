#ifndef PROJECT_HXX
#define PROJECT_HXX

#include <string>

#include <dll-export.h>

// Include all public headers
#include <deduplicate.hxx>
#include <log.hxx>

namespace project {

/** @brief Get project version string, e.g. "1.0.0"
 */
DLL auto version() -> std::string const&;

}  // namespace project

#endif  // PROJECT_HXX
