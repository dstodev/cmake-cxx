find_package(Eigen3 REQUIRED)
find_package(HelpGTestLibrary REQUIRED)

help_gtest_library(links_gtest
	SOURCES
		link_gtest.cxx
)

help_gtest_library(links_eigen
	SOURCES
		link_eigen.cxx

	LIBRARIES
		Eigen3::Eigen
)
