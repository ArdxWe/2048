#ifndef DEMO_SURFACE_H
#define DEMO_SURFACE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <cstdint>
#include <memory>
#include <string>

#include "Font.h"

class Window;
class Surface {
 public:
  explicit Surface(const std::string &path);
  Surface(Font &font, const std::string &text, SDL_Color color);
  SDL_Surface *get();
  int getWidth();
  int getHeight();

 private:
  struct Deleter {
    void operator()(SDL_Surface *p);
  };
  std::unique_ptr<SDL_Surface, Deleter> surface_;
};

#endif  // DEMO_SURFACE_H
