//
// Created by ardxwe on 2021/2/6.
//

#include "Application.h"

#include <algorithm>
#include <cassert>
#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <iomanip>
#include <mutex>
#include <stdexcept>
#include <string>
#include <vector>

namespace {
using std::array;
using std::condition_variable;
using std::getenv;
using std::move;
using std::mutex;
using std::runtime_error;
using std::sort;
using std::streamsize;
using std::string;
using std::stringstream;
using std::unique_lock;
using std::unique_ptr;
using std::vector;
using Rect = Renderer::Rect;
using namespace std::string_literals;

constexpr const int FPS = 80;
constexpr const int SCORES_SIZE = 18;
constexpr const int AUTHOR_SIZE = 15;
constexpr const Renderer::Rgba BACKGROUND = {135, 206, 255, 255};
constexpr const char *FONTS_CMD = "find /usr/share/fonts -name '*.ttf'";
constexpr const char *IMAGE_CMD = "find ../res/image -name '*.png'";
constexpr const char *MUSIC_PATH = "../res/music/hd.mp3";
constexpr const char *ICON_PATH = "../res/image/icon.png";
constexpr const Window::Size INIT_SIZE{720, 540};

stringstream executeCmd(const string &cmd) {
  auto close = [](FILE *file) { pclose(file); };
  unique_ptr<FILE, decltype(close)> pipe{popen(cmd.c_str(), "r"), close};

  vector<char> buff(0x100);
  size_t n;
  stringstream stream;

  while ((n = fread(buff.data(), sizeof(buff[0]), buff.size(), pipe.get())) >
         0) {
    stream.write(buff.data(), static_cast<streamsize>(n));
  }
  return stream;
}

Window createWindow() {
  Window window{"2048"s,
                0x2FFF0000,  // center
                0x2FFF0000,  // center
                INIT_SIZE, static_cast<uint32_t>(SDL_WINDOW_SHOWN)};
  return window;
}

Texture createTextureFromSurface(Renderer &renderer, Surface &surface) {
  return Texture{SDL_CreateTextureFromSurface(renderer.get(), surface.get())};
}

Font createFont(int size) {
  stringstream stream = executeCmd(FONTS_CMD);
  string path;
  getline(stream, path);
  if (path.empty()) {
    throw runtime_error{"find no fonts."s};
  }
  return Font{path, size};
}

vector<Surface> getResImages() {
  vector<string> paths;

  stringstream stream = executeCmd(IMAGE_CMD);
  string path;
  while (getline(stream, path)) {
    paths.emplace_back(move(path));
  }

  sort(paths.begin(), paths.end());
  vector<Surface> res;
  res.reserve(paths.size());
  for (auto &p : paths) {
    res.emplace_back(p);
  }
  return res;
}

int log(int x) {
  if (x == 0) return 0;

  int res = -1;
  while (x != 0) {
    x = x / 2;
    res++;
  }
  return res;
}

void setIcon(Window &window, Surface &icon) {
  SDL_SetWindowIcon(window.get(), icon.get());
}
}  // namespace

Application::Application()
    : images_{getResImages()},
      window_{createWindow()},
      renderer_{window_},
      size_{window_.getSize()},
      music_{MUSIC_PATH},
      engine_{device_()},
      icon_{ICON_PATH} {
  setIcon(window_, icon_);
  renderer_.setColor(BACKGROUND);
  updateLocations(size_, locations_);
}

void Application::run() {
  music_.play();
  Font big{createFont(SCORES_SIZE)};
  Font small{createFont(AUTHOR_SIZE)};
  bool quit = false;
  SDL_Event e;
  const uint8_t *keyStates;
  string text = "scores: "s;
  string author = "by ardxwe"s;
  Rect dst;
  double frame_tm;
  std::chrono::high_resolution_clock::time_point now;
  while (!quit) {
    now = std::chrono::high_resolution_clock::now();
    renderer_.clear();
    control(KeyState::OTHER);
    while (SDL_PollEvent(&e) != 0) {
      switch (e.type) {
        case SDL_QUIT: {
          quit = true;
          break;
        }
        case SDL_KEYDOWN:
          keyStates = SDL_GetKeyboardState(nullptr);
          switch (state_) {
            case State::STOPPING:
              if (keyStates[SDL_SCANCODE_S]) {
                int first = getRandom();
                int second = getRandom();
                copyTexture(1, first);
                copyTexture(1, second);
                state_ = State::RUNNING;
              }
              break;
            case State::RUNNING: {
              if (keyStates[SDL_SCANCODE_UP]) {
                control(KeyState::UP);
                if (!noZero()) {
                  copyTexture(1, getRandom());
                }
                view();
              } else if (keyStates[SDL_SCANCODE_DOWN]) {
                control(KeyState::DOWN);
                if (!noZero()) {
                  copyTexture(1, getRandom());
                }
                view();
              } else if (keyStates[SDL_SCANCODE_LEFT]) {
                control(KeyState::LEFT);
                if (!noZero()) {
                  copyTexture(1, getRandom());
                }
                view();
              } else if (keyStates[SDL_SCANCODE_RIGHT]) {
                control(KeyState::RIGHT);
                if (!noZero()) {
                  copyTexture(1, getRandom());
                }
                view();
              } else {
                control(KeyState::OTHER);
              }

              if (end()) {
                {
                  Surface over{
                      big,
                      "game over and your scores: "s + std::to_string(scores_),
                      {0xFF, 0xFF, 0xFF}};

                  dst = {(size_.w - over.getWidth()) / 2,
                         (size_.h - over.getHeight()) / 2, over.getWidth(),
                         over.getHeight()};
                  Texture new_t{createTextureFromSurface(renderer_, over)};
                  renderer_.copyTexture(new_t, nullptr, &dst);
                }
                state_ = State::STOPPING;
                for (auto &item : map_) {
                  item = 0;
                }
                scores_ = 0;
                renderer_.renderPresent();
                SDL_Delay(1000);
                break;
              }
            }
            default:
              break;
          }
      }
    }
    view();
    // scores
    {
      Surface surface{big, text + std::to_string(scores_), {0, 0, 0}};
      if (size_.h > size_.w) {
        dst = {(size_.w - surface.getWidth()) / 2,
               size_.w + (size_.h - size_.w - surface.getHeight()) / 2,
               surface.getWidth(), surface.getHeight()};
      } else {
        dst = {size_.h + (size_.w - size_.h - surface.getWidth()) / 2,
               (size_.h - surface.getHeight()) / 2, surface.getWidth(),
               surface.getHeight()};
      }
      Texture t{createTextureFromSurface(renderer_, surface)};
      renderer_.copyTexture(t, nullptr, &dst);
    }

    // author
    {
      Surface surface{small, author, {70, 130, 138}};
      dst = {size_.w - surface.getWidth(), size_.h - surface.getHeight(),
             surface.getWidth(), surface.getHeight()};
      Texture t{createTextureFromSurface(renderer_, surface)};
      renderer_.copyTexture(t, nullptr, &dst);
    }
    renderer_.renderPresent();
    frame_tm = std::chrono::duration_cast<std::chrono::duration<double>>(
                   std::chrono::high_resolution_clock::now() - now)
                   .count();
    if (frame_tm < 1000.0 / FPS) {
      SDL_Delay(static_cast<uint32_t>(1000.0 / FPS - frame_tm));
    }
  }
}

int Application::getRandom() {
  int res;
  do {
    res = r_(engine_);
  } while (map_[res] != 0);
  map_[res] = 2;
  return res;
}

void Application::copyTexture(int image_index, int location_index) {
  Texture t{createTextureFromSurface(renderer_, images_[image_index])};
  renderer_.copyTexture(t, nullptr,
                        &locations_[location_index / 4][location_index % 4]);
}

void Application::control(KeyState state) {
  vector<int> nums(4);
  switch (state) {
    case KeyState::UP:
      for (size_t j = 0; j < 4; j++) {
        nums[0] = map_[j];
        nums[1] = map_[4 + j];
        nums[2] = map_[8 + j];
        nums[3] = map_[12 + j];

        nums = merge(nums);

        map_[j] = nums[0];
        map_[4 + j] = nums[1];
        map_[8 + j] = nums[2];
        map_[12 + j] = nums[3];
      }
      break;
    case KeyState::DOWN:
      for (size_t j = 0; j < 4; j++) {
        nums[0] = map_[12 + j];
        nums[1] = map_[8 + j];
        nums[2] = map_[4 + j];
        nums[3] = map_[j];

        nums = merge(nums);

        map_[12 + j] = nums[0];
        map_[8 + j] = nums[1];
        map_[4 + j] = nums[2];
        map_[j] = nums[3];
      }
      break;
    case KeyState::LEFT:
      for (size_t j = 0; j < 4; j++) {
        nums[0] = map_[4 * j];
        nums[1] = map_[1 + 4 * j];
        nums[2] = map_[2 + 4 * j];
        nums[3] = map_[3 + 4 * j];

        nums = merge(nums);

        map_[4 * j] = nums[0];
        map_[1 + 4 * j] = nums[1];
        map_[2 + 4 * j] = nums[2];
        map_[3 + 4 * j] = nums[3];
      }
      break;
    case KeyState::RIGHT:
      for (size_t j = 0; j < 4; j++) {
        nums[0] = map_[3 + 4 * j];
        nums[1] = map_[2 + 4 * j];
        nums[2] = map_[1 + 4 * j];
        nums[3] = map_[4 * j];

        nums = merge(nums);

        map_[3 + 4 * j] = nums[0];
        map_[2 + 4 * j] = nums[1];
        map_[1 + 4 * j] = nums[2];
        map_[4 * j] = nums[3];
      }
      break;
    case KeyState::OTHER:
      break;
  }
}

void Application::view() {
  for (int i = 0; i < map_.size(); i++) {
    copyTexture(log(map_[i]), static_cast<int>(i));
  }
}

vector<int> Application::merge(std::vector<int> &nums) {
  assert(nums.size() == 4);
  vector<int> res;
  size_t count = 0;
  for (int &num : nums) {
    if (num != 0) {
      count++;
      res.push_back(num);
    }
  }
  for (size_t i = 0; i < (4 - count); i++) {
    res.push_back(0);
  }

  int value;
  switch (count) {
    case 0:
    case 1:
      break;
    case 2:
      if (res[0] == res[1]) {
        res[0] += res[0];
        value = res[0];
        scores_ += value;
        res[1] = 0;
      }
      break;
    case 3:
      if (res[0] == res[1]) {
        res[0] += res[0];
        value = res[0];
        scores_ += value;
        res[1] = res[2];
        res[2] = 0;
      } else if (res[1] == res[2]) {
        res[1] += res[1];
        value = res[1];
        scores_ += value;
        res[2] = 0;
      }
      break;
    case 4:
      if (res[0] == res[1]) {
        if (res[2] == res[3]) {
          res[0] += res[1];
          res[1] = res[2] + res[3];
          scores_ += res[0] + res[1];
          res[2] = 0;
          res[3] = 0;
        } else {
          res[0] += res[0];
          value = res[0];
          scores_ += value;
          res[1] = res[2];
          res[2] = res[3];
          res[3] = 0;
        }
      } else if (res[1] == res[2]) {
        res[1] += res[1];
        value = res[1];
        scores_ += value;
        res[2] = res[3];
        res[3] = 0;
      } else if (res[2] == res[3]) {
        res[2] += res[2];
        value = res[2];
        scores_ += value;
        res[3] = 0;
      }
      break;
    default:
      break;
  }
  return res;
}

void Application::updateLocations(
    Window::Size &size,
    std::array<std::array<Renderer::Rect, 4>, 4> &locations) {
  int len = size.h > size.w ? size.w : size.h;
  int step = len / 25;
  int rect_len = step * 5;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      locations[i][j].x = (j + 1) * step + j * rect_len;
      locations[i][j].y = (i + 1) * step + i * rect_len;
      locations[i][j].width = rect_len;
      locations[i][j].height = rect_len;
    }
  }
}

bool Application::end() {
  bool res = noZero();

  if (res) {
    for (int i = 0; i < 16; i++) {
      if (i / 4 < 3) {
        if (map_[i] == map_[i + 4]) {
          res = false;
        }
      }
      if (i % 4 < 3) {
        if (map_[i] == map_[i + 1]) {
          res = false;
        }
      }
    }
  }
  return res;
}

bool Application::noZero() {
  bool res = true;
  for (auto &item : map_) {
    if (item == 0) {
      res = false;
      break;
    } else {
      res = true;
    }
  }
  return res;
}
