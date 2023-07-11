#ifndef TEXTURE_HXX
#define TEXTURE_HXX

#include <functional>
#include <memory>
#include <utility>

#include <SDL_render.h>

namespace project {

class Texture
{
public:
	using Initializer = std::function<SDL_Texture*(SDL_Renderer*)>;

	explicit Texture(Initializer initializer);

	void init(SDL_Renderer* renderer);

	[[nodiscard]]
	auto data() const -> SDL_Texture*;

	[[nodiscard]]
	int width() const;

	[[nodiscard]]
	int height() const;

private:
	Initializer _initializer;
	std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)> _texture;
	int _width;
	int _height;
};

}  // namespace project

#endif  // TEXTURE_HXX
