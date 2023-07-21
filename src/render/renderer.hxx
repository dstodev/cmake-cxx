#ifndef RENDERER_HXX
#define RENDERER_HXX

#include <project-api.h>

struct SDL_Renderer;

namespace project {

template <typename T>
struct point_t;

struct EventHandler;
struct Simulation;
struct Player;

/*  This class cannot be const-correct because the SDL render functions do not
    take const pointers.
*/
class PROJECT_API Renderer
{
public:
	Renderer(EventHandler const& handler);

	void init(SDL_Renderer* renderer);
	void deinit();

	void refresh();
	void render();
	void clear();

	void draw(Simulation const& simulation);

protected:
	void draw(Player const& player);
	void draw(point_t<int> const& point);

private:
	SDL_Renderer* _renderer;
	EventHandler const& _handler;

	Simulation const* _simulation;
};

}  // namespace project

#endif
