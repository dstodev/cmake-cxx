set(export_name "project")

message(DEBUG "CPack variables defined BEFORE project:")
log_all_vars(FILTER CPACK MODE DEBUG)

set(CPACK_GENERATOR "TXZ")
set(CPACK_THREADS 0)  # https://cmake.org/cmake/help/latest/module/CPack.html#variable:CPACK_THREADS
set(CPACK_ARCHIVE_THREADS 0)
set(CPACK_VERBATIM_VARIABLES TRUE)

include(CPack)  # https://cmake.org/cmake/help/latest/module/CPack.html

set(CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install-stage"
	CACHE PATH "When building the `install` target, put artifacts in this directory."
)  # https://cmake.org/cmake/help/latest/variable/CMAKE_INSTALL_PREFIX.html

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

install(IMPORTED_RUNTIME_ARTIFACTS GLEW::GLEW RUNTIME DESTINATION bin)

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

# Generate & install project config file
string(JOIN "\n" file_content
	"include(\"\${CMAKE_CURRENT_LIST_DIR}/${export_name}-targets.cmake\")"
	"" # Empty line
)
file(GENERATE OUTPUT ${export_name}Config.cmake CONTENT ${file_content})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${export_name}Config.cmake" DESTINATION cmake)

# Export CMake targets
install(EXPORT ${export_name}
	FILE ${export_name}-targets.cmake
	DESTINATION cmake
	NAMESPACE project::
)

if(APPLE)
	# https://cmake.org/cmake/help/latest/module/BundleUtilities.html
	install(CODE "
		include(BundleUtilities)
		fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/bin/app-gui.app\" \"\" \"\${CMAKE_INSTALL_PREFIX}/lib\")
	")
endif()
