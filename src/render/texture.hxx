#ifndef TEXTURE_HXX
#define TEXTURE_HXX

#include <functional>
#include <memory>

#include <SDL_render.h>

#include <dll-export.h>
#include <point_t.hxx>

namespace project {

class DLL Texture
{
public:
	using Initializer = std::function<SDL_Texture*(SDL_Renderer*)>;

	explicit Texture(Initializer initializer);

	void init(SDL_Renderer* renderer);
	void reset_color();

	auto data() const -> SDL_Texture*;
	int width() const;
	int height() const;

	SDL_Rect rect() const;
	SDL_Rect rect_centered(int x, int y) const;
	SDL_Rect rect_centered(point_t<int> const& point) const;

private:
	Initializer _initializer;
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> _texture;
	int _width;
	int _height;
};

}  // namespace project

#endif  // TEXTURE_HXX
