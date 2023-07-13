#ifndef RENDER_SPECS_HXX
#define RENDER_SPECS_HXX

#include <project-api.h>

#include <application-impl.hxx>
#include <player.hxx>
#include <point_t.hxx>
#include <simulation.hxx>

struct SDL_Renderer;

namespace project {

PROJECT_API void draw(SDL_Renderer* renderer, ApplicationImpl const& application);
PROJECT_API void draw(SDL_Renderer* renderer, Simulation const& simulation);
PROJECT_API void draw(SDL_Renderer* renderer, Player const& player);
PROJECT_API void draw(SDL_Renderer* renderer, point_t<int> const& point);

}  // namespace project

#endif  // RENDER_SPECS_HXX
