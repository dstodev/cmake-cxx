include_guard()

#[[
	find_substrings(out_count out_pos_list substring in_string)

	Find all occurrences of substring within in_string.


	Parameters
	----------

	out_pos_list :
		Name of variable to store list of occurrence positions.

	substring :
		Substring to search for.

	in_string :
		String to search in.


	Options
	-------

	LAST :
		If set, the string is searched backwards.

		This is useful to disambiguate e.g:

			in_string: "((("
			substring: "(("

		Should the result index be "0" or "1"?
		Default returns 0, LAST returns 1.
]]
function(find_substrings out_pos_list substring in_string)
	cmake_parse_arguments(PARSE_ARGV 3 args "LAST" "" "")

	if(args_LAST)
		set(reverse REVERSE)
	endif()

	string(LENGTH "${substring}" substr_len)

	set(offset 0)

	while(TRUE)
		string(FIND "${in_string}" "${substring}" pos ${reverse})

		if(pos EQUAL -1)
			break()
		endif()

		if(args_LAST)
			list(APPEND pos_list ${pos})
			string(SUBSTRING "${in_string}" 0 "${pos}" in_string)
		else()
			math(EXPR offset "${offset} + ${pos}")
			list(APPEND pos_list ${offset})
			math(EXPR offset "${offset} + ${substr_len}")
			math(EXPR pos "${pos} + ${substr_len}")
			string(SUBSTRING "${in_string}" "${pos}" -1 in_string)
		endif()
	endwhile()

	if(args_LAST)
		list(REVERSE pos_list)
	endif()

	set(${out_pos_list} "${pos_list}" PARENT_SCOPE)
endfunction()

include(Expect)
expect_test_preamble()

function(test_find_substrings)
	set(input "<>one<>two<><>three<><><>")
	find_substrings(pos_list "<>" "${input}")
	expect(DEFINED pos_list)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 7)
	expect("${pos_list}" STREQUAL "0;5;10;12;19;21;23")
endfunction()
test_find_substrings()

function(test_find_substrings_last)
	set(input "<>one<>two<><>three<><><>" LAST)
	find_substrings(pos_list "<>" "${input}")
	expect(DEFINED pos_list)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 7)
	expect("${pos_list}" STREQUAL "0;5;10;12;19;21;23")
endfunction()
test_find_substrings_last()

function(test_find_substrings_none)
	set(input "<>one<>two<><>three<><><>")
	find_substrings(pos_list "()" "${input}")
	expect(DEFINED pos_list)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 0)
	expect("${pos_list}" STREQUAL "")
endfunction()
test_find_substrings_none()

function(test_find_substrings_none_last)
	set(input "<>one<>two<><>three<><><>" LAST)
	find_substrings(pos_list "()" "${input}")
	expect(DEFINED pos_list)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 0)
	expect("${pos_list}" STREQUAL "")
endfunction()
test_find_substrings_none_last()

function(test_find_substrings_match_first_simple)
	set(input "<<<one<<<")
	find_substrings(pos_list "<<" "${input}")
	expect(DEFINED pos_list)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 2)
	expect("${pos_list}" STREQUAL "0;6")
endfunction()
test_find_substrings_match_first_simple()

function(test_find_substrings_match_first)
	set(input "<<<<<one<<<<<")
	find_substrings(pos_list "<<" "${input}")
	expect(DEFINED pos_list)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 4)
	expect("${pos_list}" STREQUAL "0;2;8;10")
endfunction()
test_find_substrings_match_first()

function(test_find_substrings_match_last_simple)
	set(input "<<<one<<<")
	find_substrings(pos_list "<<" "${input}" LAST)
	expect(DEFINED pos_list)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 2)
	expect("${pos_list}" STREQUAL "1;7")
endfunction()
test_find_substrings_match_last_simple()

function(test_find_substrings_match_last)
	set(input "<<<<<one<<<<<")
	find_substrings(pos_list "<<" "${input}" LAST)
	expect(DEFINED pos_list)
	list(LENGTH pos_list pos_list_len)
	expect(${pos_list_len} EQUAL 4)
	expect("${pos_list}" STREQUAL "1;3;9;11")
endfunction()
test_find_substrings_match_last()
