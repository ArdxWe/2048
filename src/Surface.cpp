//
// Created by ardxwe on 2021/2/3.
//

#include "Surface.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include <stdexcept>

namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

Surface::Surface(const std::string &path) : surface_{IMG_Load(path.c_str())} {
  if (surface_ == nullptr) {
    throw runtime_error{"Error calling IMG_Load: "s + IMG_GetError()};
  }
}

Surface::Surface(Font &font, const std::string &text, SDL_Color color)
    : surface_{TTF_RenderText_Solid(font.get(), text.c_str(), color)} {}

SDL_Surface *Surface::get() { return surface_.get(); }

void Surface::Deleter::operator()(SDL_Surface *p) { SDL_FreeSurface(p); }

int Surface::getWidth() { return surface_->w; }

int Surface::getHeight() { return surface_->h; }
