include_guard()

#[[
	string_capitalize(input_string result) uppercases the first letter of input_string,
	lowercases all other letters, then stores the result in result.
]]
function(string_capitalize in_string out_var)
	string(SUBSTRING ${in_string} 0 1 first_letter)
	string(SUBSTRING ${in_string} 1 -1 rest_of_string)
	string(TOUPPER ${first_letter} first_letter)
	string(TOLOWER ${rest_of_string} rest_of_string)
	set(${out_var} "${first_letter}${rest_of_string}" PARENT_SCOPE)
endfunction()

expect_test_preamble()

function(test_capitalize_lower_string)
	string_capitalize("lower" out)
	expect("Lower" STREQUAL out)
endfunction()
test_capitalize_lower_string()

function(test_capitalize_upper_string)
	string_capitalize("UPPER" out)
	expect("Upper" STREQUAL out)
endfunction()
test_capitalize_upper_string()
