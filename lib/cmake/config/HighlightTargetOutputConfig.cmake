find_package(AlignStrings CONFIG REQUIRED)

#[[
	Register a print command which prints the target output path & directory
	after the target is built.
]]
function(highlight_target_output target)
	set(str1 "Built ${target}: file:///$<TARGET_FILE:${target}>")
	set(str2 "To directory: file:///$<TARGET_FILE_DIR:${target}>")

	align_strings(str1 str2 ":")

	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E echo "${str1}"
		COMMAND ${CMAKE_COMMAND} -E echo "${str2}"
	)
endfunction()
