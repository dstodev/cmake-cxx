add_google_executable(link-gtest TEST SOURCES gtest.cxx)
add_google_executable(link-fmt TEST SOURCES fmt.cxx LIBRARIES fmt::fmt)

add_local_all_target(all-link)
