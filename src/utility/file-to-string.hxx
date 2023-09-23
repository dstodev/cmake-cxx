#ifndef FILE_TO_STRING_HXX
#define FILE_TO_STRING_HXX

#include <filesystem>
#include <string>

#include <dll-export.h>

namespace project {

DLL std::string file_to_string(std::filesystem::path const& path);

}  // namespace project

#endif  // FILE_TO_STRING_HXX
