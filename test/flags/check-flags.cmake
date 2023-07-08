set(file_path ${COMPILE_COMMANDS_JSON})  # passed as script parameter

if(NOT EXISTS ${file_path})
	string(JOIN "\n" msg
		"Target 'check-flags' requires generator support for compile_commands.json"
		" Did not find: ${file_path}"
	)
	message(FATAL_ERROR ${msg})
endif()

file(STRINGS ${file_path} lines REGEX "command.*_flags")
list(LENGTH lines num_lines)
math(EXPR stop "${num_lines} - 1")

foreach(line IN LISTS lines)
	string(REGEX REPLACE "^.*: \"(.*_flags.*)\".*$" "\\1" line "${line}")  # Focus on command line
	string(REGEX REPLACE "[ \t]+" " " line "${line}")  # Remove extra spaces
	string(REPLACE "\\\\" "\\" line "${line}")  # Remove double backslashes
	string(APPEND msg " ${line}\n")
endforeach()

message(${msg})
