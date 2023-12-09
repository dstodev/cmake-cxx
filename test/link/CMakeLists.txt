add_google_executable(link-cxxopts TEST SOURCES cxxopts.cxx LIBRARIES cxxopts::cxxopts)
add_google_executable(link-fmt TEST SOURCES fmt.cxx LIBRARIES fmt::fmt)
add_google_executable(link-gtest TEST SOURCES gtest.cxx)

add_local_all_target(all-link)
