include_guard()

include(LogVars)

function(log_policy policy)
	if(POLICY ${policy})
		cmake_policy(GET ${policy} ${policy})
	else()
		set(${policy} "NOTFOUND")
	endif()

	log_vars(${policy} ${ARGN})
endfunction()
