#ifndef RENDER_SPECS_HXX
#define RENDER_SPECS_HXX

#include <project-api.h>

#include <application-impl.hxx>
#include <game.hxx>
#include <player.hxx>
#include <point_t.hxx>

struct SDL_Renderer;

PROJECT_API void draw(SDL_Renderer* renderer, project::ApplicationImpl const& application);
PROJECT_API void draw(SDL_Renderer* renderer, project::Game const& game);
PROJECT_API void draw(SDL_Renderer* renderer, project::Player const& player);
PROJECT_API void draw(SDL_Renderer* renderer, project::point_t const& point);

#endif  // RENDER_SPECS_HXX
