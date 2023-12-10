function(message_gate_open)
	enable_message(TRUE)
endfunction()

function(message_gate_close)
	enable_message(FALSE)
endfunction()

function(enable_message predicate)
	if(predicate)
		set(value TRUE)  # convert all truthy values to TRUE
	else()
		set(value FALSE)  # and all falsy values to FALSE
	endif()
	set(__MESSAGE_GATE_OPEN ${value} CACHE INTERNAL "Emit messages?")
endfunction()

expect_test_preamble()

set(__MESSAGE_GATE_OPEN TRUE CACHE INTERNAL "Emit messages?")

function(message)
	cmake_parse_arguments(PARSE_ARGV 0 args "FORCE" "" "")
	if(__MESSAGE_GATE_OPEN OR args_FORCE)
		_message(${args_UNPARSED_ARGUMENTS})
	endif()
endfunction()
