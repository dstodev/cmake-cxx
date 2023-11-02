# https://www.libsdl.org/
# https://github.com/libsdl-org/SDL

CPMAddPackage(NAME sdl
	GITHUB_REPOSITORY libsdl-org/SDL
	GIT_TAG release-2.26.5
	OPTIONS "SDL2_DISABLE_INSTALL ON" "SDL2_DISABLE_UNINSTALL ON" "SDL_TEST OFF"
	EXCLUDE_FROM_ALL TRUE
	SYSTEM TRUE
)
