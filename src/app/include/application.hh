#ifndef _APPLICATION_H_
#define _APPLICATION_H_

#include <SDL3/SDL.h>
#include "renderer.hh"

class Application {
    public:

    ~Application() noexcept;

    Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    Application(Application&&) noexcept = default;
    Application& operator=(Application&&) noexcept = default;

    auto run() -> void;

    private:
    auto handle_events() -> void;

    auto delta_time() -> void;


    SDL_Window* window_{nullptr};

    float delta_time_{};
    bool running_{true};
    bool is_minimized_{false};

}

#endif // _APPLICATION_H_
