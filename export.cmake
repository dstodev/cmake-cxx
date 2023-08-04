set(export_name "project")

# Package the application
install(TARGETS
	# Project targets
	project

	# Optional targets
	app-gui
	app-console

	# Dependency targets
	SDL2
	SDL2main
	eigen
	fmt

	EXPORT ${export_name}
	BUNDLE DESTINATION bin
)
install(DIRECTORY "${eigen_SOURCE_DIR}/Eigen" DESTINATION include)
install(DIRECTORY "${SDL2_SOURCE_DIR}/include/" DESTINATION include/SDL2)
install(DIRECTORY "${fmt_SOURCE_DIR}/include/" DESTINATION include)

# Redistribute system runtimes
set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries)

set(release_runtimes ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
list(FILTER release_runtimes EXCLUDE REGEX "Debug")

set(debug_runtimes ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
list(FILTER debug_runtimes INCLUDE REGEX "Debug")

install(PROGRAMS ${release_runtimes} DESTINATION bin CONFIGURATIONS Release)
install(PROGRAMS ${debug_runtimes} DESTINATION bin CONFIGURATIONS Debug)

log_vars(release_runtimes debug_runtimes SPLIT_LISTS MODE DEBUG)

# Export CMake targets
install(EXPORT ${export_name}
	DESTINATION cmake
	NAMESPACE project::
)

if (APPLE)
	# https://cmake.org/cmake/help/latest/module/BundleUtilities.html
	install(CODE "
		include(BundleUtilities)
		fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/bin/app-gui.app\" \"\" \"\${CMAKE_INSTALL_PREFIX}/lib\")
	")
endif()
