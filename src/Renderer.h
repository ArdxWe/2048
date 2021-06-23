#ifndef DEMO_RENDERER_H
#define DEMO_RENDERER_H

#include <SDL2/SDL.h>

#include <memory>

class Window;
class Texture;
class Renderer {
 public:
  struct Rect {
    int x;
    int y;
    int width;
    int height;
  };
  struct Rgba {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
  };
  explicit Renderer(const Window &window);
  void setColor(const Rgba &rgba);
  SDL_Renderer *get();
  void clear();
  void copyTexture(Texture &texture, Rect *src, Rect *dst);
  void copyAllTexture(Texture &texture);
  void renderPresent();

 private:
  struct Deleter {
    void operator()(SDL_Renderer *p);
  };
  std::unique_ptr<SDL_Renderer, Deleter> renderer_;
};

#endif  // DEMO_RENDERER_H
