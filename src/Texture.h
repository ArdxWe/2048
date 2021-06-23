#ifndef DEMO_TEXTURE_H
#define DEMO_TEXTURE_H

#include <SDL2/SDL.h>

#include <memory>

class Surface;
class Texture {
 public:
  Texture() = default;
  explicit Texture(SDL_Texture *x);
  SDL_Texture *get();
  void setBlendMode(SDL_BlendMode mode);
  void setAlpha(uint8_t alpha);

 private:
  struct Deleter {
    void operator()(SDL_Texture *p);
  };
  std::unique_ptr<SDL_Texture, Deleter> texture_;
};

#endif  // DEMO_TEXTURE_H
