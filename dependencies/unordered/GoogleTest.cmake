# https://github.com/google/googletest
# for testing

#[[
	If GoogleTest is taking a long time to add, it may be because it's trying
	to find a Python interpreter. After I installed Python, GoogleTest
	configuration time improved from around 7 seconds to under 1 second.
]]

CPMAddPackage(NAME googletest
	GITHUB_REPOSITORY google/googletest
	VERSION 1.14.0
	OPTIONS "BUILD_GMOCK OFF" "INSTALL_GTEST OFF" "gtest_force_shared_crt ON"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
