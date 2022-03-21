find_package(Eigen3 REQUIRED)

set(target_name "shear")

add_library(${target_name}
	shear.cxx
	shear.hxx
)
set_target_properties(${target_name}
	PROPERTIES
		PUBLIC_HEADER "shear.hxx"  # Can also be a list of headers!
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

install(TARGETS ${target_name}
	EXPORT ${target_name}
)
install(EXPORT ${target_name}
	DESTINATION cmake
	NAMESPACE shear::
)

# Include Eigen headers in the package
install(DIRECTORY "$<TARGET_PROPERTY:Eigen3::Eigen,INTERFACE_INCLUDE_DIRECTORIES>/Eigen"
	DESTINATION include
)
