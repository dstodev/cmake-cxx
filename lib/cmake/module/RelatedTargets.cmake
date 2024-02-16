include_guard()

include(DirectoryTargets)

#[[
	Returns a list (in out_var) of all targets defined in the same directory as the given target.
	If RECURSIVE is given, the list will also include targets defined in subdirectories.
]]
function(related_targets out_var target)
	get_property(target_dir TARGET ${target} PROPERTY SOURCE_DIR)
	directory_targets(targets ${target_dir} ${ARGN})
	set(${out_var} ${targets} PARENT_SCOPE)
endfunction()
