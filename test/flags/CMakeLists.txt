set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)  # https://cmake.org/cmake/help/latest/prop_tgt/WINDOWS_EXPORT_ALL_SYMBOLS.html

add_custom_target(_flags)

foreach(lang c cxx)
	set(lib _flags-${lang}-lib)
	set(exe _flags-${lang}-exe)

	add_library(${lib} EXCLUDE_FROM_ALL lib.${lang})
	add_executable(${exe} EXCLUDE_FROM_ALL main.${lang})
	target_link_libraries(${exe} PRIVATE ${lib})
	add_dependencies(_flags ${lib} ${exe})
	set_target_properties(${lib} ${exe} PROPERTIES OUTPUT_NAME flags-${lang})
endforeach()

if(CMAKE_GENERATOR MATCHES "NMake Makefiles")
	set(verbose /nologo /p)
else()
	set(verbose VERBOSE=1)
endif()

add_custom_target(check-flags
	${CMAKE_COMMAND} --build ${PROJECT_BINARY_DIR} --clean-first --target _flags -- ${verbose}
)
