include_guard()

#[[
	CMake does not provide a way to detect library target aliases.
	Instead, override add_library() to record alias definitions.

	This is a heavy-handed approach--may only override add_library() once.

	After calling this function, the following variables are available:
	- ${out_names} - name of the variable that contains the list of alias names
	- ${out_targets} - name of the variable that contains the list of alias targets

	e.g.
		record_alias_definitions(ALIAS_NAMES ALIAS_TARGETS)
		# ...
		message("Alias names: ${${ALIAS_NAMES}}")
		message("Alias targets: ${${ALIAS_TARGETS}}")

	Repeatedly calling this function updates output variables accordingly.
]]
function(record_alias_definitions out_names out_targets)
	set(__out_names_var "__${out_names}" CACHE INTERNAL "")
	set(__out_targets_var "__${out_targets}" CACHE INTERNAL "")
	set(${__out_names_var} "" CACHE INTERNAL "")
	set(${__out_targets_var} "" CACHE INTERNAL "")
	set(${out_names} "${__out_names_var}" PARENT_SCOPE)
	set(${out_targets} "${__out_targets_var}" PARENT_SCOPE)
endfunction()

function(add_library)
	list(FIND ARGN ALIAS alias_index)
	# The following variables contain parameter indices:
	#          name_index   alias_index  target_index
	#             0|            1|            2|
	#             -+---------  --+--  ---------+-
	# add_library(Alias::Name  ALIAS  some_target)

	math(EXPR last_index "${ARGC} - 1")

	if(alias_index GREATER 0 AND alias_index LESS ${last_index})
		math(EXPR name_index "${alias_index} - 1")
		math(EXPR target_index "${alias_index} + 1")
		list(GET ARGN ${name_index} alias_name)
		list(GET ARGN ${target_index} alias_target)
		_record_alias(${alias_name} ${alias_target})
	endif()

	_add_library(${ARGN})  # Forward all arguments to normal add_library() unchanged
endfunction()

function(_record_alias alias_name alias_target)
	message(VERBOSE "Detected library alias: ${alias_name} -> ${alias_target}")
	set(alias_names "${${__out_names_var}}")
	set(alias_targets "${${__out_targets_var}}")
	list(APPEND alias_names ${alias_name})
	list(APPEND alias_targets ${alias_target})
	set(${__out_names_var} "${alias_names}" CACHE INTERNAL "")
	set(${__out_targets_var} "${alias_targets}" CACHE INTERNAL "")
endfunction()
