find_package(HelpGTestLibrary REQUIRED)

help_gtest_library(shear_test
	SOURCES
		test_shear.cxx

	LIBRARIES
		shear
)
