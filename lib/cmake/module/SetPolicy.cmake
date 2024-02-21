include_guard()

function(set_policy policy value)
	if(POLICY ${policy})
		cmake_policy(SET ${policy} ${value})
	endif()
endfunction()
