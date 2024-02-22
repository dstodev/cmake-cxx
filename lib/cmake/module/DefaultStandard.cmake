include_guard()

function(default_standard)
	cmake_parse_arguments(PARSE_ARGV 0 args "" "C;CXX" "")

	if(args_KEYWORDS_MISSING_VALUES)
		list(JOIN args_KEYWORDS_MISSING_VALUES " " msg)
		message(FATAL_ERROR "Missing value for parameters: ${msg}")
	endif()

	if(args_C)
		_set_standard(C ${args_C})
		set(CMAKE_C_STANDARD ${CMAKE_C_STANDARD} PARENT_SCOPE)
		set(CMAKE_C_STANDARD_REQUIRED ${CMAKE_C_STANDARD_REQUIRED} PARENT_SCOPE)
	endif()

	if(args_CXX)
		_set_standard(CXX ${args_CXX})
		set(CMAKE_CXX_STANDARD ${CMAKE_CXX_STANDARD} PARENT_SCOPE)
		set(CMAKE_CXX_STANDARD_REQUIRED ${CMAKE_CXX_STANDARD_REQUIRED} PARENT_SCOPE)
	endif()
endfunction()

function(_set_standard lang version)
	if(NOT DEFINED CMAKE_${lang}_STANDARD)
		get_cmake_property(enabled_languages ENABLED_LANGUAGES)

		if(NOT lang IN_LIST enabled_languages)
			message(FATAL_ERROR "Language not enabled: ${lang}")
		endif()

		get_cmake_property(all_features CMAKE_${lang}_KNOWN_FEATURES)
		set(supported_features "${CMAKE_${lang}_COMPILE_FEATURES}")

		string(TOLOWER "${lang}" feature_prefix)
		list(FILTER all_features INCLUDE REGEX "${feature_prefix}_std_")
		list(FILTER supported_features INCLUDE REGEX "${feature_prefix}_std_")

		list(JOIN supported_features " " supported_features_str)
		set(requested_feature "${feature_prefix}_std_${version}")

		if(NOT requested_feature IN_LIST all_features)
			string(JOIN "\n " msg " "
				"${lang} standard (${version}) is not valid!"
				"Supported features: ${supported_features_str}"
				"")
			message(FATAL_ERROR "${msg}")
		endif()

		if(NOT requested_feature IN_LIST supported_features)
			string(JOIN "\n " msg " "
				"${lang} standard (${version}) is valid, but not supported by your compiler!"
				"Compiler ID: ${CMAKE_${lang}_COMPILER_ID}"
				"Compiler version: ${CMAKE_${lang}_COMPILER_VERSION}"
				"Supported features: ${supported_features_str}"
				"")
			message(FATAL_ERROR "${msg}")
		endif()

		set(CMAKE_${lang}_STANDARD ${version} PARENT_SCOPE)
		set(CMAKE_${lang}_STANDARD_REQUIRED TRUE PARENT_SCOPE)
	endif()
endfunction()
