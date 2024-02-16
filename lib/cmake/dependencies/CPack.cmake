# This file is included first, before all other dependencies.

set(CPACK_GENERATOR "TXZ")
set(CPACK_THREADS 0)  # https://cmake.org/cmake/help/latest/module/CPack.html#variable:CPACK_THREADS
set(CPACK_ARCHIVE_THREADS 0)
set(CPACK_VERBATIM_VARIABLES TRUE)

include(CPack)  # https://cmake.org/cmake/help/latest/module/CPack.html
