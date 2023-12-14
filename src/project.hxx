#ifndef PROJECT_HXX
#define PROJECT_HXX

#include <string>

#include <dll-export.h>

namespace project {

DLL auto version() -> std::string const&;

}  // namespace project

#endif  // PROJECT_HXX
