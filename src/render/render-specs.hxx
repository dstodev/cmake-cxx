#ifndef RENDER_SPECS_HXX
#define RENDER_SPECS_HXX

#include <project-api.h>

#include <application-impl.hxx>
#include <event-handler.hxx>
#include <simulation.hxx>

struct SDL_Renderer;

namespace project {

/*  Draw functions share the name "draw", but have different signatures based
    on what they are drawing. The parameters are ordered as follows:
      1.  The object to draw.
      2.  The SDL_Renderer to draw with.
      3+. Extra parameters, if any.
 */

PROJECT_API void draw(ApplicationImpl const& application, SDL_Renderer* renderer);
PROJECT_API void draw(Simulation const& simulation, SDL_Renderer* renderer, EventHandler const& handler);

}  // namespace project

#endif  // RENDER_SPECS_HXX
