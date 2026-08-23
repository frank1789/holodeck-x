#include "application.hh"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include "renderer.hh"

#include <chrono>
#include <stdexcept>

Application::~Application() noexcept {
  vkDeviceWaiIdle(renderer_.get_context().device);

  SDL_DestroyWindow(window_);
}

Application::Application() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    const auto msg_err =
        fmt::format("Failed to initiliase SDL {}", SDL_GetError());
    throw std::runtime_error(msg_err);
  }

  const float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags = SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE |
                                 SDL_WINDOW_HIDDEN |
                                 SDL_WINDOW_HIGH_PIXEL_DENSITY;
  const auto width = static_cast<int>(1280 * main_scale);
  const auto height = static_cast<int>(800 * main_scale);
  SDL_Window* window =
      SDL_CreateWindow("CalculiX", width, height, window_flags);
  if (window == nullptr) {
    const auto msg_err =
        fmt::format("[ERROR] Failed to create SDL window: {}", SDL_GetError());
    throw std::runtime_error(msg_err);
  }

  // renderer
  // ui_renderer
}

auto Application::run() -> void {
  SDL_ShowWindow(window_);
  while (running_) {
    handle_events();
    delta_time();
  }
}

auto Application::handle_events() -> void {
  for (SDL_Event event; SDL_PollEvent(&event);) {
    ImGui_ImplSDL3_ProcessEvent(&event);
    switch (event.type) {
      case SDL_EVENT_QUIT:
        running_ = false;
        break;

      case SDL_EVENT_WINDOW_MINIMIZED:
        is_minimized_ = true;
        break;

      case SDL_EVENT_WINDOW_RESTORED:
        is_minimized_ = false;
        break;

      case SDL_EVENT_WINDOW_RESIZED:
        // render
        break;
    }
  }
}

auto Application::delta_time() -> void {
  static auto previous_time = std::chrono::steady_clock::now();
  const auto current_time = std::chrono::steady_clock::now();
  delta_time_ =
      std::chrono::duration<float>(current_time - previous_time).count();
  previous_time = current_time;
}
