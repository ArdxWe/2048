//
// Created by ardxwe on 2/7/21.
//

#include "Image.h"

#include <SDL2/SDL_image.h>

#include <stdexcept>

namespace {
using std::runtime_error;
using namespace std::string_literals;
}  // namespace

Image::Image() {
  uint flags = IMG_INIT_PNG;
  if (!(static_cast<uint>(IMG_Init(flags)) & flags)) {
    throw runtime_error{"Error call IMG_Init: "s + SDL_GetError()};
  }
}

Image::~Image() { IMG_Quit(); }
