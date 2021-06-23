//
// Created by ardxwe on 2/7/21.
//

#include "Font.h"

#include <SDL2/SDL_ttf.h>

Font::Font(const std::string &path, int size)
    : font_{TTF_OpenFont(path.c_str(), size)} {}

TTF_Font *Font::get() { return font_.get(); }

void Font::Deleter::operator()(TTF_Font *font) { TTF_CloseFont(font); }
