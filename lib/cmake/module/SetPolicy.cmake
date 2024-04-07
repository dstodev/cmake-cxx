include_guard()

include(LogVars)

function(set_policy policy value)
	if(POLICY ${policy})
		cmake_policy(GET ${policy} before)
		cmake_policy(SET ${policy} ${value})
		cmake_policy(GET ${policy} after)
	else()
		set(before "NOTFOUND")
		set(after "${before}")
	endif()

	if (before STREQUAL after)
		set(${policy} "${before}")
	else()
		set(${policy} "${before} -> ${after}")
	endif()

	log_vars(${policy} MODE DEBUG)
endfunction()
