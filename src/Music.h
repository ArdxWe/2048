#ifndef DEMO_MUSIC_H
#define DEMO_MUSIC_H

#include <SDL2/SDL_mixer.h>

#include <memory>
#include <string>

class Music {
 public:
  explicit Music(const std::string &);
  ~Music();
  void play();

 private:
  struct Deleter {
    void operator()(Mix_Music *);
  };
  std::unique_ptr<Mix_Music, Deleter> _p{};
};

#endif  // DEMO_MUSIC_H
