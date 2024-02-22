include_guard()

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
	set(__message_gate_open ${value} CACHE INTERNAL "Emit messages?")
endfunction()

include_guard(GLOBAL)

message_gate_open()

function(message)
	cmake_parse_arguments(PARSE_ARGV 0 args "FORCE" "" "")
	if(__message_gate_open OR args_FORCE)
		_message(${args_UNPARSED_ARGUMENTS})
	endif()
endfunction()
