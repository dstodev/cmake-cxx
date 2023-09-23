#ifndef RENDERER_HXX
#define RENDERER_HXX

#include <functional>

#include <vao-square.hxx>

struct SDL_Window;
using SDL_GLContext = void*;  // from SDL_video.h

namespace project {

template <typename T>
class Point;

class EventHandler;
class Player;
class Simulation;

/* This class cannot be const-correct because the SDL render functions do not
   take const pointers.
 */
class Renderer
{
public:
	explicit Renderer(EventHandler const& handler);
	~Renderer();

	Renderer(Renderer const& copy) = delete;
	Renderer(Renderer&& move) = default;

	Renderer& operator=(Renderer const& copy) = delete;
	Renderer& operator=(Renderer&& move) = default;

	void init(SDL_Window* window);
	void deinit();

	void refresh();
	void render();
	void clear();

	void draw(Simulation const& simulation);

protected:
	void draw(Player const& player, Point<int> const& view_center);
	void draw(Point<int> const& point);

private:
	std::reference_wrapper<EventHandler const> _handler;
	SDL_Window* _window;
	SDL_GLContext _context;

	void compile_shaders();
	unsigned int _shader_program;

	vao::Square as_square;
};

}  // namespace project

#endif
