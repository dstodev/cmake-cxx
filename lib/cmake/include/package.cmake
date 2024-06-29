set(export_name "project")

# Package CMake targets for the application to import
install(TARGETS
	# Project targets
	project

	# Dependency targets
	cxxopts  # for sample
	fmt      # for logging

	EXPORT ${export_name}

	ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

	PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
	BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Package sample application executable
install(FILES $<TARGET_FILE:app-console> DESTINATION bin)

# Redistribute dependency headers
install(DIRECTORY "${CPM_PACKAGE_cxxopts_SOURCE_DIR}/include/" DESTINATION include FILES_MATCHING PATTERN "*.h*")
install(DIRECTORY "${CPM_PACKAGE_fmt_SOURCE_DIR}/include/" DESTINATION include)

# Redistribute system runtimes
set(CMAKE_INSTALL_DEBUG_LIBRARIES TRUE)
set(CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS_SKIP TRUE)
include(InstallRequiredSystemLibraries)

set(release_runtimes ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
list(FILTER release_runtimes EXCLUDE REGEX "Debug")

set(debug_runtimes ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS})
list(FILTER debug_runtimes INCLUDE REGEX "Debug")

if(release_runtimes OR debug_runtimes)
	install(PROGRAMS ${release_runtimes} DESTINATION bin CONFIGURATIONS Release)
	install(PROGRAMS ${debug_runtimes} DESTINATION bin CONFIGURATIONS Debug)
	log_vars(release_runtimes debug_runtimes MODE DEBUG)
endif()

# Generate & install project config file
string(JOIN "\n" file_content
	"include(\"\${CMAKE_CURRENT_LIST_DIR}/${export_name}-targets.cmake\")"
	"")  # Empty line

file(GENERATE OUTPUT "${export_name}-config.cmake" CONTENT ${file_content})
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${export_name}-config.cmake" DESTINATION cmake)

# Export CMake targets
install(EXPORT ${export_name}
	FILE ${export_name}-targets.cmake
	DESTINATION cmake
	NAMESPACE project::
)

unset(export_name)
unset(release_runtimes)
unset(debug_runtimes)
unset(file_content)
