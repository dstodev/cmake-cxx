#ifndef TEXTURE_HXX
#define TEXTURE_HXX

#include <functional>
#include <memory>

#include <SDL_render.h>
#include <project-api.h>

namespace project {

class Texture
{
public:
	using Initializer = std::function<SDL_Texture*(SDL_Renderer*)>;

	PROJECT_API explicit Texture(Initializer initializer);

	PROJECT_API void init(SDL_Renderer* renderer);

#define _attr [[nodiscard]] PROJECT_API
	_attr auto data() const -> SDL_Texture*;
	_attr int width() const;
	_attr int height() const;

	_attr SDL_Rect rect() const;
	_attr SDL_Rect rect_centered(int x, int y) const;
#undef _attr

private:
	Initializer _initializer;
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> _texture;
	int _width;
	int _height;
};

}  // namespace project

#endif  // TEXTURE_HXX
