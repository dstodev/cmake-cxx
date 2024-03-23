include_guard()

#[[
	find_substrings(out_count out_pos_list substring in_string)

	Find all occurrences of substring within in_string.

	Parameters
	----------
	out_count :
		Name of variable to store number of substring occurrences.

	out_pos_list :
		Name of variable to store list of occurrence positions.

	substring :
		Substring to search for.

	in_string :
		String to search in.
]]
function(find_substrings out_count out_pos_list substring in_string)
	string(LENGTH "${substring}" substr_len)

	set(count 0)
	set(offset 0)

	while(TRUE)
		string(FIND "${in_string}" "${substring}" pos)
		if(pos EQUAL -1)
			break()
		endif()
		math(EXPR offset "${offset} + ${pos}")
		list(APPEND pos_list ${offset})
		math(EXPR offset "${offset} + ${substr_len}")
		math(EXPR count "${count} + 1")
		math(EXPR pos "${pos} + ${substr_len}")
		string(SUBSTRING "${in_string}" "${pos}" -1 in_string)
	endwhile()

	set(${out_count} ${count} PARENT_SCOPE)
	set(${out_pos_list} "${pos_list}" PARENT_SCOPE)
endfunction()

include(Expect)
expect_test_preamble()

function(test_find_substrings)
	set(input "<>one<>two<><>three<><><>")
	find_substrings(count pos_list "<>" "${input}")
	expect(DEFINED count)
	expect(DEFINED pos_list)
	expect(${count} EQUAL 7)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 7)
	expect("${pos_list}" STREQUAL "0;5;10;12;19;21;23")
endfunction()
test_find_substrings()

function(test_find_substrings_none)
	set(input "<>one<>two<><>three<><><>")
	find_substrings(count pos_list "()" "${input}")
	expect(DEFINED count)
	expect(DEFINED pos_list)
	expect(${count} EQUAL 0)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 0)
	expect("${pos_list}" STREQUAL "")
endfunction()
test_find_substrings_none()
