#ifndef RENDER_SPECS_HXX
#define RENDER_SPECS_HXX

#include <project-api.h>

#include <application-impl.hxx>
#include <event-handler.hxx>
#include <simulation.hxx>

struct SDL_Renderer;

namespace project {

PROJECT_API void draw(SDL_Renderer* renderer, ApplicationImpl const& application);
PROJECT_API void draw(SDL_Renderer* renderer, Simulation const& simulation, EventHandler const& handler);

}  // namespace project

#endif  // RENDER_SPECS_HXX
