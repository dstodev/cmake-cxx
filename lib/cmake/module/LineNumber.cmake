include_guard()

#[[
	Return in out_var the line number of the first line in file that matches pattern.
	If no line matches, out_var contains -1.
]]
function(line_number out_var file pattern)
	set(${out_var} -1 PARENT_SCOPE)

	file(READ ${file} data)
	string(REGEX REPLACE ";" "<semicolon>" data "${data}")
	string(REGEX REPLACE "#\\[\\[|\\]\\]" "<comment-block-bound>" data "${data}")
	string(REGEX REPLACE "\r\n?|\n" ";" lines "${data}")

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

include(Expect)
expect_test_preamble()

function(test_line_number)
	set(pattern "function\\(test_line_number\\)")
	set(line ${CMAKE_CURRENT_FUNCTION_LIST_LINE})
	set(file "${CMAKE_CURRENT_FUNCTION_LIST_FILE}")

	line_number(line_number ${file} "${pattern}")

	expect(32 EQUAL ${line_number})
	expect(${line} EQUAL ${line_number})
endfunction()
test_line_number()
