#ifndef RENDERER_HXX
#define RENDERER_HXX

struct SDL_Renderer;

namespace project {

template <typename T>
class point_t;

class EventHandler;
class Player;
class Simulation;

/*  This class cannot be const-correct because the SDL render functions do not
    take const pointers.
*/
class Renderer
{
public:
	explicit Renderer(EventHandler const& handler);

	Renderer(Renderer const& copy) = delete;
	Renderer(Renderer&& move) = default;

	Renderer& operator=(Renderer const& copy) = delete;
	Renderer& operator=(Renderer&& move) = default;

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
	EventHandler const& _handler;
	Simulation const* _simulation;
	SDL_Renderer* _renderer;
};

}  // namespace project

#endif
