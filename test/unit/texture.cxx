#include <gtest/gtest.h>

#include <texture.hxx>

using namespace project;

TEST(Texture, construct)
{
	Texture texture([](SDL_Renderer*) { return nullptr; });
	(void) texture;
}

TEST(Texture, init_nullptr)
{
	Texture texture([](SDL_Renderer*) { return nullptr; });
	texture.init(nullptr);
}

TEST(Texture, dimensions)
{
	Texture texture([](SDL_Renderer* renderer) {
		return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 9, 9);
	});
	auto window = SDL_CreateWindow("", 0, 0, 0, 0, 0);
	auto renderer = SDL_CreateRenderer(window, -1, 0);
	texture.init(renderer);
	ASSERT_EQ(9, texture.width());
	ASSERT_EQ(9, texture.height());
}

#define ASSERT_SDL_RECT_EQUALS(expected, actual) \
	do { \
		auto lhs = expected; \
		auto rhs = actual; \
		ASSERT_TRUE(SDL_RectEquals(&lhs, &rhs)) \
		    << "Expected: " << lhs.x << ", " << lhs.y << ", " << lhs.w << ", " << lhs.h << "\n" \
		    << "Actual: " << rhs.x << ", " << rhs.y << ", " << rhs.w << ", " << rhs.h; \
	} while (0)

TEST(Texture, rectangles)
{
	Texture texture([](SDL_Renderer* renderer) {
		return SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 9, 9);
	});
	auto window = SDL_CreateWindow("", 0, 0, 0, 0, 0);
	auto renderer = SDL_CreateRenderer(window, -1, 0);
	texture.init(renderer);

	ASSERT_SDL_RECT_EQUALS((SDL_Rect {0, 0, 9, 9}), texture.rect());
	ASSERT_SDL_RECT_EQUALS((SDL_Rect {0, 0, 9, 9}), texture.rect_centered(4, 4));
	ASSERT_SDL_RECT_EQUALS((SDL_Rect {1, 1, 9, 9}), texture.rect_centered(5, 5));
}
