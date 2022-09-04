set(target_name main)

find_package(Qt6 REQUIRED)
find_package(glib REQUIRED)

set(CMAKE_AUTOMOC ON)

add_executable(${target_name}
	main.cxx
)
target_link_libraries(${target_name}
	PRIVATE
		project
		Qt6::Widgets
		Qt6::QWindowsIntegrationPlugin
)

# Copy QT platform libs https://doc.qt.io/qt-6/windows-deployment.html
add_custom_command(TARGET ${target_name} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${Qt6_QWindowsIntegrationPlugin_LIB_DIRS}
	"$<TARGET_FILE_DIR:${target_name}>/platforms"  # The name of the folder MUST be "platforms" for the program to run
)
