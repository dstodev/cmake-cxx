#include "texture.hxx"

namespace project {

Texture::Texture(Initializer initializer)
    : _initializer(std::move(initializer))
    , _texture(nullptr, &SDL_DestroyTexture)
    , _width(0)
    , _height(0)
{}

void Texture::init(SDL_Renderer* renderer)
{
	_texture.reset(_initializer(renderer));
	SDL_QueryTexture(_texture.get(), nullptr, nullptr, &_width, &_height);
}

auto Texture::data() const -> SDL_Texture*
{
	return _texture.get();
}

int Texture::width() const
{
	return _width;
}

int Texture::height() const
{
	return _height;
}

}  // namespace project
