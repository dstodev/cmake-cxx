add_google_executable(link-cxxopts SOURCES cxxopts.cxx LIBRARIES cxxopts::cxxopts)
add_google_executable(link-fmt SOURCES fmt.cxx LIBRARIES fmt::fmt)
add_google_executable(link-gtest SOURCES gtest.cxx)

add_local_all_target(all-link)
