target_sources(${target_name}
	PRIVATE
		grid-t.hxx
		log.cxx
		log.hxx
		notify-assignment.hxx
		placeholder-t.hxx
		point-t.hxx
		priority-mutex-t.cxx
		priority-mutex-t.hxx
		shear.cxx
		shear.hxx
		thread-pool-t.hxx
)
target_include_directories(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
)
target_compile_definitions(${target_name}
	PUBLIC
		$<$<CONFIG:Debug>:ENABLE_LOGGING>
)

#[[
	This call to target_precompile_headers() affects development builds
	(i.e. not a distribution package/install context) by prepending
	'#include <placeholder.hxx>' to all sources, exposing the
	placeholder_t type for developer utility.

	This behavior applies in a BUILD_INTERFACE context and not in a
	INSTALL_INTERFACE context, meaning that:

	The project::placeholder_t type is usable on-the-fly while developing,
	as the header is implicitly provided. However, the header is not supplied
	to packages, so packages still using this type will fail to compile.

	This signals that the package should not release; the placeholder type
	should first be replaced with a real type. Therefore, the 'placeholder'
	name and 'type not found' error should make sense in context.
]]
target_precompile_headers(${target_name}
	PUBLIC
		$<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/placeholder-t.hxx>
)
