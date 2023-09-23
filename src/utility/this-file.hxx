#ifndef THIS_FILE_HXX
#define THIS_FILE_HXX

#include <filesystem>

#define THIS_FILE()      (std::filesystem::path(__FILE__))
#define THIS_DIRECTORY() (std::filesystem::path(__FILE__).parent_path())

#endif  // THIS_FILE_HXX
