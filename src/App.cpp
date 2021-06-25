//
// Created by ardxwe on 2021/1/31.
//

#include "App.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <stdexcept>
#include <string>

namespace {
using namespace std::string_literals;
using std::runtime_error;
}  // namespace

App::App() {
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
    throw runtime_error{"Error call SDL_Init: "s + SDL_GetError()};
  }
  int flags = IMG_INIT_PNG;
  if (!(static_cast<uint>(IMG_Init(flags)) & flags)) {
    throw runtime_error{"Error call IMG_Init: "s + SDL_GetError()};
  }
}

App::~App() { SDL_Quit(); }
