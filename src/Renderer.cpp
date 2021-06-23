//
// Created by ardxwe on 2021/2/5.
//

#include "Renderer.h"

#include <SDL2/SDL.h>

#include <stdexcept>

#include "Texture.h"
#include "Window.h"

namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

Renderer::Renderer(const Window &window)
    : renderer_{
          SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED)} {
  if (renderer_ == nullptr) {
    throw runtime_error{"Error call SDL_CreateRenderer: "s + SDL_GetError()};
  }
}

void Renderer::setColor(const Rgba &rgba) {
  SDL_SetRenderDrawColor(renderer_.get(), rgba.r, rgba.g, rgba.b, rgba.a);
}

SDL_Renderer *Renderer::get() { return renderer_.get(); }

void Renderer::Deleter::operator()(SDL_Renderer *p) { SDL_DestroyRenderer(p); }

void Renderer::clear() { SDL_RenderClear(renderer_.get()); }

void Renderer::copyTexture(Texture &texture, Rect *src, Rect *dst) {
  SDL_RenderCopy(renderer_.get(), texture.get(),
                 reinterpret_cast<SDL_Rect *>(src),
                 reinterpret_cast<SDL_Rect *>(dst));
}

void Renderer::copyAllTexture(Texture &texture) {
  SDL_RenderCopy(renderer_.get(), texture.get(), nullptr, nullptr);
}

void Renderer::renderPresent() { SDL_RenderPresent(renderer_.get()); }
