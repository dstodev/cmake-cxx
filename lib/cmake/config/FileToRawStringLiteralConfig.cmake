find_package(HelpParseArguments CONFIG REQUIRED)

#[[
	Converts a file to a raw string literal (>=C++11: https://en.cppreference.com/w/cpp/language/string_literal)
]]
function(file_to_raw_string_literal file_path)
	help_parse_arguments(args "" "CHAR_SEQ;OUTPUT_DIRECTORY;TARGET" "")

	if(args_CHAR_SEQ)
		set(char_seq "${args_CHAR_SEQ}")
	else()
		set(char_seq "#<file-content>#")
	endif()

	if(args_OUTPUT_DIRECTORY)
		set(out_dir "${args_OUTPUT_DIRECTORY}")
	else()
		set(out_dir "${CMAKE_CURRENT_BINARY_DIR}")
	endif()

	get_filename_component(file_name "${file_path}" NAME)

	string(TOLOWER "${file_name}" symbol)
	string(REGEX REPLACE "[^a-z0-9_]" "_" symbol "${symbol}")
	string(TOUPPER "${symbol}_HXX" include_guard)

	string(REGEX REPLACE "[^a-z0-9\-]" "-" header "${file_name}")
	string(REGEX REPLACE "[^a-z0-9\-]" "-" source "${file_name}")

	set(header "${header}.hxx")
	set(source "${source}.cxx")

	file(READ "${file_path}" file_content)

	string(JOIN "\n" declaration
		"#ifndef ${include_guard}"
		"#define ${include_guard}\n"
		"extern char const ${symbol}[];\n"
		"#endif  // ${include_guard}\n"
	)
	string(JOIN "\n" definition
		"#include <${header}>\n"
		"char const ${symbol}[] = R\"${char_seq}("
		"${file_content}"
		")${char_seq}\";\n"
	)

	file(WRITE "${out_dir}/${header}" "${declaration}")
	file(WRITE "${out_dir}/${source}" "${definition}")

	if(args_TARGET)
		target_sources(${args_TARGET} PRIVATE
			"${out_dir}/${header}"
			"${out_dir}/${source}"
		)
		target_include_directories(${args_TARGET} PRIVATE "${out_dir}")
	endif()

	set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS ${file_path})
endfunction()
