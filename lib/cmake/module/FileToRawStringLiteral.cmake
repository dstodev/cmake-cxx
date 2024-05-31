include_guard()

include(HelpParseArguments)

#[[
	Converts a file to a raw string literal (>=C++11):
	https://en.cppreference.com/w/cpp/language/string_literal

	For example, given a path to a text file with content:

		Hello, world!
		This is my file.

	Generates a C++ raw string literal:

		char const file_content[] = R"#<file-content>#(
		Hello, world!
		This is my file.
		)#<file-content>#";


	Parameters
	----------

	file_path :
		Path to the file to convert.


	Options
	-------

	CHAR_SEQ :
		Character sequence to use for the raw string literal.
		Defaults to "#<file-content>#"

	OUTPUT_DIRECTORY :
		Directory to write the generated files to.
		Defaults to the current binary directory.

	TARGET <target> :
		Adds generated sources to <target>.
]]
function(file_to_raw_string_literal file_path)
	help_parse_arguments(args "" "CHAR_SEQ;OUTPUT_DIRECTORY;TARGET;API_INCLUDE;API_DEFINE" "")

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

	if(args_API_INCLUDE AND args_API_DEFINE)
		set(api_include "#include <${args_API_INCLUDE}>\n")
		set(api_define "${args_API_DEFINE} ")
	endif()

	get_filename_component(file_name "${file_path}" NAME)  # /path/to/My_File.txt -> My_File.txt

	string(TOLOWER "${file_name}" symbol)  # My_File.txt -> my_file.txt
	string(REGEX REPLACE "[^a-z0-9_]" "_" symbol "${symbol}")  # my_file.txt -> my_file_txt
	string(TOUPPER "${symbol}_HXX" include_guard)  # my_file_txt -> MY_FILE_TXT_HXX

	string(REGEX REPLACE "[^a-z0-9\-]" "-" header "${symbol}")  # my_file_txt -> my-file-txt
	string(REGEX REPLACE "[^a-z0-9\-]" "-" source "${symbol}")

	set(header "${header}.hxx")  # my-file-txt -> my-file-txt.hxx
	set(source "${source}.cxx")

	file(READ "${file_path}" file_content)

	string(JOIN "\n" declaration
		"#ifndef ${include_guard}"
		"#define ${include_guard}\n"
		${api_include}
		"${api_define}extern char const ${symbol}[];\n"
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
		target_include_directories(${args_TARGET} PRIVATE "$<BUILD_INTERFACE:${out_dir}>")
	endif()

	set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${file_path}")
endfunction()
