find_package(Eigen3 REQUIRED)

set(target_name "shear")

add_library(${target_name}
	shear.cxx
	shear.hxx
)
target_include_directories(${target_name}
	PUBLIC  # Can be INTERFACE with relative includes e.g. #include "shear.hxx"
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
		$<INSTALL_INTERFACE:include>
)
target_compile_features(${target_name}
	PUBLIC
		cxx_std_11
)
target_link_libraries(${target_name}
	PUBLIC
		Eigen3::Eigen
)
