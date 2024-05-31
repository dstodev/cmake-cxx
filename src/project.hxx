#ifndef PROJECT_HXX
#define PROJECT_HXX

#include <string_view>

#include <project_dll-export.h>

// Include all public headers
#include <deduplicate.hxx>
#include <log.hxx>

namespace project {

/** @brief Get project version string, e.g. "1.0.0"
 */
DLL auto version() -> std::string_view;

}  // namespace project

#endif  // PROJECT_HXX
