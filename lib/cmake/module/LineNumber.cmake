include_guard()

#[[
	Return in out_var the line number of the first line in file that matches pattern.
	If no line matches, out_var contains -1.
]]
function(line_number out_var file pattern)
	set(${out_var} -1 PARENT_SCOPE)

	file(STRINGS ${file} lines)
	list(LENGTH lines line_count)
	math(EXPR line_count "${line_count} - 1")

	foreach(line_index RANGE ${line_count})
		list(GET lines ${line_index} line)

		if(line MATCHES "${pattern}")
			math(EXPR line_index "${line_index} + 1")  # line number starts at 1
			set(${out_var} ${line_index} PARENT_SCOPE)
			return()
		endif()
	endforeach()
endfunction()
