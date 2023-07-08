#ifndef READER_HXX
#define READER_HXX

#include <project-api.h>

namespace project::input {

PROJECT_API void start_reader_thread();
PROJECT_API void stop_reader_thread();

}  // namespace project::input

#endif  // READER_HXX
