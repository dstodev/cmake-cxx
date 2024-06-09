include_guard()

include(FindSubstrings)

#[[
	balanced_match(out_var left_delimiter right_delimiter in_str)

	Matches all text within the outermost pair of balanced delimiters
	in a string.

	Stores in out_var the substring between the first occurrence of the
	left delimiter and the paired occurrence of the right delimiter.

	If no balanced pair is found, out_var is undefined.
]]
function(balanced_match out_var left_delimiter right_delimiter in_str)
	find_substrings(lefts "${left_delimiter}" "${in_str}")
	find_substrings(rights "${right_delimiter}" "${in_str}" LAST)

	if(NOT lefts OR NOT rights)
		return()
	endif()

	# Remove right delimiters that appear before the first left delimiter
	list(GET lefts 0 left)
	foreach(right IN LISTS rights)
		if(right LESS left)
			list(REMOVE_ITEM rights ${right})
		endif()
	endforeach()

	# Remove left delimiters that appear after the last right delimiter
	list(GET rights -1 right)
	foreach(left IN LISTS lefts)
		if(left GREATER right)
			list(REMOVE_ITEM lefts ${left})
		endif()
	endforeach()

	list(LENGTH lefts num_lefts)
	list(LENGTH rights num_rights)

	if(NOT num_lefts EQUAL num_rights)
		return()
	endif()

	set(left_depth 0)
	set(left_pos -1)
	set(right_pos -1)

	while(left_depth GREATER -1)
		# Delimiter positions are in order of appearance from left to right
		list(GET lefts ${left_depth} left)

		if(left_pos EQUAL -1)
			# Latch first left delimiter position
			set(left_pos ${left})
		endif()

		# Next right delimiter
		list(GET rights 0 right)

		# Look ahead to see if there is a closer left delimiter
		math(EXPR next_depth "${left_depth} + 1")
		list(LENGTH lefts num_lefts)

		if(num_lefts GREATER next_depth)
			list(GET lefts ${next_depth} next_left)

			if(next_left LESS right)
				# Skip the current left delimiter
				math(EXPR left_depth "${left_depth} + 1")
				continue()
			endif()
		endif()

		# Now we know the right delimiter closes the current left delimiter
		list(REMOVE_AT lefts ${left_depth})
		math(EXPR left_depth "${left_depth} - 1")
		list(POP_FRONT rights right)
		set(right_pos ${right})
	endwhile()

	string(LENGTH "${left_delimiter}" left_len)
	math(EXPR left_pos "${left_pos} + ${left_len}")
	math(EXPR length "${right_pos} - ${left_pos}")

	string(SUBSTRING "${in_str}" ${left_pos} ${length} out)
	set(${out_var} "${out}" PARENT_SCOPE)
endfunction()

include(Expect)
expect_test_preamble()

function(test_balanced_match_simple)
	set(input "some(thing)else")
	balanced_match(out "(" ")" "${input}")
	expect(DEFINED out)
	expect("${out}" STREQUAL "thing")
endfunction()
test_balanced_match_simple()

function(test_balanced_match_nested)
	set(input "some((thing)else)")
	balanced_match(out "(" ")" "${input}")
	expect(DEFINED out)
	expect("${out}" STREQUAL "(thing)else")
endfunction()
test_balanced_match_nested()

function(test_balanced_match_triple_nested)
	set(input "(some((thing)else))")
	balanced_match(out "(" ")" "${input}")
	expect(DEFINED out)
	expect("${out}" STREQUAL "some((thing)else)")
endfunction()
test_balanced_match_triple_nested()

function(test_balanced_match_two_nested)
	set(input "(some(thing)(else))")
	balanced_match(out "(" ")" "${input}")
	expect(DEFINED out)
	expect("${out}" STREQUAL "some(thing)(else)")
endfunction()
test_balanced_match_two_nested()

function(test_balanced_match_no_match)
	set(input "some(thing else")
	balanced_match(out "(" ")" "${input}")
	expect(NOT DEFINED out)

	set(input "some thing)else")
	balanced_match(out "(" ")" "${input}")
	expect(NOT DEFINED out)
endfunction()
test_balanced_match_no_match()

function(test_balanced_match_no_pair)
	set(input "some(t(hing)else")
	balanced_match(out "(" ")" "${input}")
	expect(NOT DEFINED out)

	set(input "some(thi)ng)else")
	balanced_match(out "(" ")" "${input}")
	expect(NOT DEFINED out)
endfunction()
test_balanced_match_no_pair()

function(test_balanced_match_first)
	set(input "some(thing)else(something)else")
	balanced_match(out "(" ")" "${input}")
	expect(DEFINED out)
	expect("${out}" STREQUAL "thing")
endfunction()
test_balanced_match_first()

function(test_balanced_match_wide_delimiters)
	set(input "(some((thing))else)")
	balanced_match(out "((" "))" "${input}")
	expect(DEFINED out)
	expect("${out}" STREQUAL "thing")
endfunction()
test_balanced_match_wide_delimiters()

function(test_balanced_match_wide_ambiguous)
	set(input "some(((thing)))else")
	balanced_match(out "((" "))" "${input}")
	expect(DEFINED out)
	expect("${out}" STREQUAL "(thing)")
endfunction()
test_balanced_match_wide_ambiguous()
