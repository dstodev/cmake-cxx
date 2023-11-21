# https://github.com/google/googletest

CPMAddPackage(NAME googletest
	GITHUB_REPOSITORY google/googletest
	VERSION 1.13.0
	OPTIONS "BUILD_GMOCK OFF" "INSTALL_GTEST OFF" "gtest_force_shared_crt ON"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
