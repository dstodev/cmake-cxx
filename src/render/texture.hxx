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

	[[nodiscard]]
	PROJECT_API auto data() const -> SDL_Texture*;

	[[nodiscard]]
	PROJECT_API int width() const;

	[[nodiscard]]
	PROJECT_API int height() const;

	[[nodiscard]]
	PROJECT_API SDL_Rect rect() const;

	[[nodiscard]]
	PROJECT_API SDL_Rect rect_centered(int x, int y) const;

private:
	Initializer _initializer;
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> _texture;
	int _width;
	int _height;
};

}  // namespace project

#endif  // TEXTURE_HXX
