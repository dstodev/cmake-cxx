#[[
	log_vars(var_names...) will print a readout of all given variables
	and their values. If a mode is provided, such as FATAL_ERROR, the message
	will use the given mode.

	Parameters
	----------
	var_names...
		The names of the variables to print

	MODE mode
		The mode to use for the message. If not provided, will use STATUS
]]
function(log_vars)
	cmake_parse_arguments(PARSE_ARGV 0 args "" "MODE" "")
	foreach(var IN LISTS args_UNPARSED_ARGUMENTS)
		set(message "${message} <> ${var}\t: ${${var}}\n")
	endforeach()
	message(${args_MODE} "${message}")
endfunction()

# For immediate developer utility, force_log_vars calls log_vars in FATAL_ERROR mode.
function(force_log_vars)
	list(APPEND ARGV "MODE" "FATAL_ERROR")
	log_vars(${ARGV})
endfunction()
