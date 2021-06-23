#ifndef DEMO_FONT_H
#define DEMO_FONT_H

#include <SDL2/SDL_ttf.h>

#include <memory>
#include <string>

class Font {
 public:
  Font(const std::string &path, int size);
  TTF_Font *get();

 private:
  struct Deleter {
    void operator()(TTF_Font *font);
  };
  std::unique_ptr<TTF_Font, Deleter> font_;
};

#endif  // DEMO_FONT_H
