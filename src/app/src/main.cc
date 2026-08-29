#include <SDL3/SDL_init.h>
#include <SDL3/SDL_log.h>
#include <SDL3/SDL_messagebox.h>

#include <cstdlib>
#include <stdexcept>

#include "application.hh"

auto main(int argc, char* argv[]) -> int {
  try {
    holodeckx::Application app;
    app.run();
  } catch (const std::exception& e) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "%s", e.what());
    SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Quit();
  return EXIT_SUCCESS;
}
