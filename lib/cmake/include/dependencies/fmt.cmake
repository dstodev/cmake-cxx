# https://github.com/fmtlib/fmt

CPMAddPackage(NAME fmt
	GITHUB_REPOSITORY fmtlib/fmt
	GIT_TAG 10.0.0
	OPTIONS "FMT_TEST OFF" "FMT_DOC OFF" "FMT_INSTALL OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
set_target_properties(fmt PROPERTIES PUBLIC_HEADER "")
