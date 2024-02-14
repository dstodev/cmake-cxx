include_guard()

macro(record_alias_definitions out_names out_targets)

	# Override add_library to record alias definitions

	set(__${out_names} "" CACHE INTERNAL "")
	set(__${out_targets} "" CACHE INTERNAL "")

	set(${out_names} "__${out_names}")
	set(${out_targets} "__${out_targets}")

	function(_record_alias alias_name alias_target)
		message(VERBOSE "Detected library alias: ${alias_name} -> ${alias_target}")
		set(alias_names ${__${out_names}})
		set(alias_targets ${__${out_targets}})
		list(APPEND alias_names ${alias_name})
		list(APPEND alias_targets ${alias_target})
		set(__${out_names} ${alias_names} CACHE INTERNAL "")
		set(__${out_targets} ${alias_targets} CACHE INTERNAL "")
	endfunction()

	function(add_library)
		list(FIND ARGN ALIAS alias_index)
		if(alias_index GREATER 0)  # add_library(My::Alias ALIAS some_target)
			#                                       0|      1|       2|
			#                                    name_index  |  target_index
			#                                           alias_index
			math(EXPR alias_name_index "${alias_index} - 1")
			math(EXPR alias_target_index "${alias_index} + 1")
			list(GET ARGN ${alias_name_index} alias_name)
			list(GET ARGN ${alias_target_index} alias_target)
			_record_alias(${alias_name} ${alias_target})
		endif()

		# use function ARGN, not macro ARGN
		set(_argn "ARGN")
		_add_library(${${_argn}})
	endfunction()
endmacro()
