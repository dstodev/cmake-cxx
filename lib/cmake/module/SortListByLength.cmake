include_guard()

function(sort_list_by_length in_list out_list)
	foreach(item IN LISTS ${in_list})
		string(LENGTH ${item} length)
		list(APPEND sorted "${length}:${item}")
	endforeach()

	list(SORT sorted COMPARE NATURAL ${ARGN})

	foreach(item IN LISTS sorted)
		string(REGEX REPLACE "^[0-9]+:" "" element ${item})
		list(APPEND output ${element})
	endforeach()

	set(${out_list} ${output} PARENT_SCOPE)
endfunction()

include(Expect)
expect_test_preamble()

function(test_sort_list_by_length_ascending)
	set(in "a" "dddd" "bb" "ccc" "eeeeeeeeee")
	sort_list_by_length(in out)
	expect("a;bb;ccc;dddd;eeeeeeeeee" STREQUAL "${out}")
endfunction()
test_sort_list_by_length_ascending()

function(test_sort_list_by_length_descending)
	set(in "a" "dddd" "bb" "ccc" "eeeeeeeeee")
	sort_list_by_length(in out ORDER DESCENDING)
	expect("eeeeeeeeee;dddd;ccc;bb;a" STREQUAL "${out}")
endfunction()
test_sort_list_by_length_descending()
