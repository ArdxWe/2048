//
// Created by ardxwe on 4/5/21.
//

#include "Music.h"

#include <iostream>
#include <stdexcept>
#include <string>

namespace {
using std::runtime_error;
using std::string;
using namespace std::string_literals;
}  // namespace

Music::Music(const std::string &file) {
  if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 1, 2048) < 0) {
    throw runtime_error{"Call Mix_OpenAudio error: "s + Mix_GetError()};
  }
  _p.reset(Mix_LoadMUS(file.c_str()));
  if (!_p) {
    throw runtime_error{"Failed to load music! SDL_mixer Error: "s +
                        Mix_GetError()};
  }
}

Music::~Music() { Mix_Quit(); }

void Music::Deleter::operator()(Mix_Music *music) { Mix_FreeMusic(music); }

void Music::play() { Mix_PlayMusic(_p.get(), -1); }