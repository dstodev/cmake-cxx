# https://github.com/fmtlib/fmt
# for string formatting

CPMAddPackage(NAME fmt
	GITHUB_REPOSITORY fmtlib/fmt
	GIT_TAG 10.2.1
	OPTIONS "FMT_TEST OFF" "FMT_DOC OFF" "FMT_INSTALL OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
set_target_properties(fmt PROPERTIES PUBLIC_HEADER "")
