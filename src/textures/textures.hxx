#ifndef TEXTURES_HXX
#define TEXTURES_HXX

#include <SDL_render.h>

namespace project::textures {

extern SDL_Texture* player;

void init(SDL_Renderer* renderer);

}

#endif  // TEXTURES_HXX
