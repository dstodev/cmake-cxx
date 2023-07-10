#ifndef RENDER_SPECS_HXX
#define RENDER_SPECS_HXX

#include <project-api.h>

#include <application-impl.hxx>
#include <player.hxx>
#include <point_t.hxx>
#include <simulation.hxx>

struct SDL_Renderer;

PROJECT_API void draw(SDL_Renderer* renderer, project::ApplicationImpl const& application);
PROJECT_API void draw(SDL_Renderer* renderer, project::Simulation const& simulation);
PROJECT_API void draw(SDL_Renderer* renderer, project::Player const& player);
PROJECT_API void draw(SDL_Renderer* renderer, project::point_t<int> const& point);

#endif  // RENDER_SPECS_HXX
