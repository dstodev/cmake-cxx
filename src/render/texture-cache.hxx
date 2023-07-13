#ifndef TEXTURE_CACHE_HXX
#define TEXTURE_CACHE_HXX

#include <SDL_render.h>

#include <texture.hxx>

namespace project::textures {

extern Texture player;
extern Texture shift;

void init_all(SDL_Renderer* renderer);

}  // namespace project::textures

#endif  // TEXTURE_CACHE_HXX
