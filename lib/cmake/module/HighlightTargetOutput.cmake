include_guard()

include(AlignStrings)

#[[
	After `target` builds, print its output path and directory.
]]
function(highlight_target_output target)
	# a "file url" is of the form: file://<host>/<path>
	# omitting <host> is equal to "localhost"
	# https://www.ietf.org/rfc/rfc1738.txt
	set(out_path "Built ${target}: file:///$<TARGET_FILE:${target}>")
	set(out_dir "To directory: file:///$<TARGET_FILE_DIR:${target}>")

	align_strings(out_path out_dir ":")

	add_custom_command(TARGET ${target} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E echo "${out_path}"
		COMMAND ${CMAKE_COMMAND} -E echo "${out_dir}"
		VERBATIM
	)
endfunction()
