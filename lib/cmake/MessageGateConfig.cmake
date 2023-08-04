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

include_guard(GLOBAL)

set(__MESSAGE_GATE_OPEN TRUE CACHE INTERNAL "Emit messages?")

function(message)
	if(__MESSAGE_GATE_OPEN)
		_message(${ARGN})
	endif()
endfunction()
