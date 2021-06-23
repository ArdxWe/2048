#ifndef DEMO_APPLICATION_H
#define DEMO_APPLICATION_H

#include <SDL2/SDL.h>

#include <random>
#include <string>
#include <vector>

#include "App.h"
#include "Image.h"
#include "Music.h"
#include "Renderer.h"
#include "Surface.h"
#include "Texture.h"
#include "Ttf.h"
#include "Window.h"

class Application {
 public:
  Application();
  void run();

 private:
  [[maybe_unused]] App app_{};
  [[maybe_unused]] Image image_app_{};
  [[maybe_unused]] Ttf ttf_app_{};
  std::vector<Surface> images_;
  Window window_;
  Renderer renderer_;
  Window::Size size_{};
  Music music_;
  std::random_device device_{};
  std::default_random_engine engine_;
  std::uniform_int_distribution<int> r_{0, 4 * 4 - 1};  // []

  std::array<std::array<Renderer::Rect, 4>, 4> locations_{};
  std::array<int, 16> map_{};

  enum class State {
    RUNNING,
    STOPPING,
  };
  State state_ = State::STOPPING;
  uint32_t scores_ = 0;
  Surface icon_;

 private:
  int getRandom();
  void copyTexture(int image_index, int location_index);
  enum class keyState {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    OTHER,
  };
  void core(keyState state);
  std::vector<int> merge(std::vector<int> &nums);
  static void updateLocations(
      Window::Size &size,
      std::array<std::array<Renderer::Rect, 4>, 4> &locations);
  bool end();
};

#endif  // DEMO_APPLICATION_H
