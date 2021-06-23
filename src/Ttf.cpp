//
// Created by ardxwe on 2/7/21.
//

#include "Ttf.h"

#include <SDL2/SDL_ttf.h>

#include <stdexcept>

namespace {
using namespace std::string_literals;
using std::runtime_error;
}  // namespace

Ttf::Ttf() {
  if (TTF_Init() == -1) {
    throw runtime_error{"Error calling TTF_Init: "s + TTF_GetError()};
  }
}

Ttf::~Ttf() { TTF_Quit(); }
