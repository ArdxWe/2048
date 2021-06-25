//
// Created by ardxwe on 2021/2/5.
//

#include "Texture.h"

#include <SDL2/SDL.h>

Texture::Texture(SDL_Texture *x) : texture_{x} {}

SDL_Texture *Texture::get() { return texture_.get(); }
void Texture::Deleter::operator()(SDL_Texture *p) { SDL_DestroyTexture(p); }
