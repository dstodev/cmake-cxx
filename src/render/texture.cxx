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

void Texture::reset_color()
{
	SDL_SetTextureColorMod(data(), 0xff, 0xff, 0xff);
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

SDL_Rect Texture::rect() const
{
	SDL_Rect rect {0, 0, _width, _height};
	return rect;
}

SDL_Rect Texture::rect_centered(int x, int y) const
{
	SDL_Rect rect {x - _width / 2, y - _height / 2, _width, _height};
	return rect;
}

}  // namespace project
