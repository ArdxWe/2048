#ifndef DEMO_WINDOW_H
#define DEMO_WINDOW_H

#include <SDL2/SDL.h>

#include <cstdint>
#include <memory>
#include <string>

class Window {
 public:
  struct Size {
    int w;
    int h;
  };
  Window(const std::string &title, int x, int y, const Size &size,
         uint32_t flags);
  [[nodiscard]] SDL_Window *get() const;
  Size getSize();
  void setMinSize(const Size &size);
  void setMaxSize(const Size &size);

 private:
  struct Deleter {
    void operator()(SDL_Window *x);
  };
  std::unique_ptr<SDL_Window, Deleter> window_;
  Size size_;
};

#endif  // DEMO_WINDOW_H
