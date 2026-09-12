#include "application.hh"

#include <ImViewGuizmo.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <imgui_internal.h>

#include <backward.hpp>
#include <chrono>
#include <memory>
#include <stdexcept>

#include "Eigen/Core"
#include "ui/dockspace_host.hh"
#include "ui/panels/analysis_tree.hh"
#include "ui/panels/group_and_selection.hh"
#include "ui/panels/main_panel.hh"
#include "ui/panels/mesh_warning.hh"
#include "ui/panels/output_log.hh"
#include "ui/panels/properties_editor.hh"
#include "ui/panels/solver_control.hh"

namespace holodeckx {

namespace {

constexpr char const* AppName{"HolodeckX"};
constexpr char const* AppIdentifier{"com.holodeck"};

}  // namespace

backward::SignalHandling sh;

Application::~Application() noexcept {
  // vkDeviceWaitIdle(renderer_->get_context().device);
  if (ImGui::GetCurrentContext() != nullptr) {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
  }

  SDL_DestroyRenderer(renderer_);
  SDL_DestroyWindow(window_);
}

Application::Application() {
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    const auto msg_err =
        fmt::format("Failed to initiliase SDL {}", SDL_GetError());
    throw std::runtime_error(msg_err);
  }

  if (!SDL_SetAppMetadata(AppName, "0.1.0", AppIdentifier)) {
    const auto msg_err =
        fmt::format("Failed to set SDL metadata {}", SDL_GetError());
    throw std::runtime_error(msg_err);
  }

  const float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags =
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
  const auto width = static_cast<int>(1280 * main_scale);
  const auto height = static_cast<int>(800 * main_scale);
  window_ = SDL_CreateWindow("HolodeckX", width, height, window_flags);
  if (window_ == nullptr) {
    const auto msg_err =
        fmt::format("Failed to create SDL window: {}", SDL_GetError());
    throw std::runtime_error(msg_err);
  }

  renderer_ = SDL_CreateRenderer(window_, nullptr);
  if (renderer_ == nullptr) {
    const auto msg_err =
        fmt::format("Failed to create SDL renderer: {}", SDL_GetError());
    throw std::runtime_error(msg_err);
  }
  SDL_SetRenderVSync(renderer_, 1);
  // ui_renderer = UIRenderer(window_);
}

auto Application::run() -> void {
  SDL_SetWindowPosition(window_, SDL_WINDOWPOS_CENTERED,
                        SDL_WINDOWPOS_CENTERED);
  SDL_ShowWindow(window_);

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |=
      ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup scaling
  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  ImGuiStyle& style = ImGui::GetStyle();
  style.ScaleAllSizes(
      main_scale);  // Bake a fixed style scale. (until we have a solution for
                    // dynamic style scaling, changing this requires resetting
                    // Style + calling this again)
  style.FontScaleDpi =
      main_scale;  // Set initial font scale. (in docking branch: using
                   // io.ConfigDpiScaleFonts=true automatically overrides this
                   // for every window depending on the current monitor)

  // ImViewGuizmo appearance -- tweak freely, this is a one-time call.
  auto& gizmoStyle = ImViewGuizmo::GetStyle();
  gizmoStyle.scale = 0.75f;
  gizmoStyle.axisColors[0] = IM_COL32(231, 76, 60, 255);   // X
  gizmoStyle.axisColors[1] = IM_COL32(46, 204, 113, 255);  // Y
  gizmoStyle.axisColors[2] = IM_COL32(52, 152, 219, 255);  // Z
  gizmoStyle.labelColor = IM_COL32(240, 240, 240, 255);
  gizmoStyle.animateSnap = true;
  gizmoStyle.snapAnimationDuration = 0.25f;

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(window_, renderer_);
  ImGui_ImplSDLRenderer3_Init(renderer_);
  const char* home_dir = std::getenv("HOME");
  const std::string font_path =
      std::string(home_dir) + "/Library/Fonts/Roboto-Regular.ttf";
  // Load Fonts
  // - If fonts are not explicitly loaded, Dear ImGui will select an embedded
  // font: either AddFontDefaultVector() or AddFontDefaultBitmap().
  //   This selection is based on (style.FontSizeBase * style.FontScaleMain *
  //   style.FontScaleDpi) reaching a small threshold.
  // - You can load multiple fonts and use ImGui::PushFont()/PopFont() to select
  // them.
  // - If a file cannot be loaded, AddFont functions will return a nullptr.
  // Please handle those errors in your code (e.g. use an assertion, display an
  // error and quit).
  // - Read 'docs/FONTS.md' for more instructions and details.
  // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use FreeType
  // for higher quality font rendering.
  // - Remember that in C/C++ if you want to include a backslash \ in a string
  // literal you need to write a double backslash \\ !
  // style.FontSizeBase = 20.0f;
  // io.Fonts->AddFontDefaultVector();
  // io.Fonts->AddFontDefaultBitmap();
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf");
  // io.Fonts->AddFontFromFileTTF("~/Library/Fonts/Roboto-Regular.ttf");
  // 1. ALWAYS load the default font first as a valid index 0 fallback
  ImFont* default_font = io.Fonts->AddFontDefault();

  // 2. Try loading your custom downloaded font
  const char* path = "/Users/francesco/Library/Fonts/Roboto-Medium.ttf";
  ImFont* roboto_font = io.Fonts->AddFontFromFileTTF(path, 16.0f);

  if (roboto_font == nullptr) {
    // The load failed, so assign our fallback pointer to protect against
    // crashes
    printf("[imgui-warning] Could not load %s. Falling back to default font.\n",
           path);
    roboto_font = default_font;
  }

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  panels_.add_panel<ui::AnalysisTreePanel>();
  panels_.add_panel<ui::GroupAndSelectionPanel>();
  panels_.add_panel<ui::PropertiesEditorPanel>();
  panels_.add_panel<ui::SolverControlPanel>();
  panels_.add_panel<ui::MeshWarningPanel>();
  panels_.add_panel<ui::OutputLogPanel>();
  auto& main = panels_.add_panel<ui::MainPanel>();
  main.set_camera(&camera_);

  while (running_) {
    {
      // Poll and handle events (inputs, window resize, etc.)
      // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to
      // tell if dear imgui wants to use your inputs.
      // - When io.WantCaptureMouse is true, do not dispatch mouse input data to
      // your main application, or clear/overwrite your copy of the mouse data.
      // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input
      // data to your main application, or clear/overwrite your copy of the
      // keyboard data. Generally you may always pass all inputs to dear imgui,
      // and hide them from your application based on those two flags. [If using
      // SDL_MAIN_USE_CALLBACKS: call ImGui_ImplSDL3_ProcessEvent() from your
      // SDL_AppEvent() function]
      SDL_Event event;
      while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL3_ProcessEvent(&event);
        if (event.type == SDL_EVENT_QUIT) {
          running_ = false;
        }

        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
            event.window.windowID == SDL_GetWindowID(window_)) {
          running_ = false;
        }
      }

      // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your
      // SDL_AppIterate() function]
      if (SDL_GetWindowFlags(window_) & SDL_WINDOW_MINIMIZED) {
        SDL_Delay(16);
        continue;
      }

      // 3. Start the Dear ImGui frame
      ImGui_ImplSDLRenderer3_NewFrame();
      ImGui_ImplSDL3_NewFrame();
      ImGuiIO& io = ImGui::GetIO();
      if (io.DisplaySize.x <= 0.0f || io.DisplaySize.y <= 0.0f) {
        int width{};
        int height{};
        SDL_GetWindowSize(window_, &width, &height);
        // If SDL reports 0, force it to 1280x720 (or your starting size)
        // so ImGui won't assert
        const auto width_f = static_cast<float>((width > 0 ? width : 1280));
        const auto height_f = static_cast<float>((height > 0 ? height : 1280));
        io.DisplaySize = ImVec2(width_f, height_f);
      }
      ImGui::NewFrame();  // 3rd: Core ImGui frame initialization (Now safe!)
      ui::DockSpaceHost::begin();
      if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Window")) {
          for (const auto& panel : panels_.panels()) {
            {
              bool open = panel->is_open();
              if (ImGui::MenuItem(panel->get_name(), nullptr, open)) {
                panel->set_open(!open);
              }
            }
          }
          ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
      }

      holodeckx::ui::DockSpaceHost::end();
      panels_.draw_all();

      // --- 5. Rendering Pipeline ---
      ImGui::Render();

      // Note: SDL3's SDL_Renderer handles high-DPI scaling automatically in
      // most setups. If your text looks blurry, keep this line; otherwise, it
      // can sometimes be omitted in SDL3.

      if (io.DisplayFramebufferScale.x <= 0.0f ||
          io.DisplayFramebufferScale.y <= 0.0f) {
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
      }
      SDL_SetRenderScale(renderer_, io.DisplayFramebufferScale.x,
                         io.DisplayFramebufferScale.y);

      SDL_SetRenderDrawColorFloat(renderer_, clear_color.x, clear_color.y,
                                  clear_color.z, clear_color.w);
      SDL_RenderClear(renderer_);

      ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer_);
      SDL_RenderPresent(renderer_);
    }
    delta_time();
  }
}

auto Application::handle_events() -> void {
  //   bool show_demo_window = true;
  //   bool show_another_window = false;
  //   ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  //   ImGuiIO& io = ImGui::GetIO();
  //  // (void)io;
  //   SDL_Event event;
  //   while (SDL_PollEvent(&event)) {
  //     ImGui_ImplSDL3_ProcessEvent(&event);
  //     if (event.type == SDL_EVENT_QUIT) running_ = true;
  //     if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
  //         event.window.windowID == SDL_GetWindowID(window_))
  //       running_ = true;
  //   }

  //   // [If using SDL_MAIN_USE_CALLBACKS: all code below would likely be your
  //   // SDL_AppIterate() function] if (SDL_GetWindowFlags(window_) &
  //   // SDL_WINDOW_MINIMIZED)
  //   // {
  //   //     SDL_Delay(10);
  //   //     continue;
  //   // }

  //   // Start the Dear ImGui frame
  //   ImGui_ImplSDLRenderer3_NewFrame();
  //   ImGui_ImplSDL3_NewFrame();
  //   ImGui::NewFrame();

  //   // 1. Show the big demo window (Most of the sample code is in
  //   // ImGui::ShowDemoWindow()! You can browse its code to learn more about
  //   Dear
  //   // ImGui!). if (show_demo_window)
  //   //     ImGui::ShowDemoWindow(&show_demo_window);

  //   // 2. Show a simple window that we create ourselves. We use a Begin/End
  //   pair
  //   // to create a named window.
  //   {
  //     static float f = 0.0f;
  //     static int counter = 0;

  //     ImGui::Begin("Hello, world!");  // Create a window called "Hello,
  //     world!"
  //                                     // and append into it.

  //     ImGui::Text("This is some useful text.");  // Display some text (you
  //     can use
  //                                                // a format strings too)
  //     ImGui::Checkbox(
  //         "Demo Window",
  //         &show_demo_window);  // Edit bools storing our window open/close
  //         state
  //     ImGui::Checkbox("Another Window", &show_another_window);

  //     ImGui::SliderFloat("float", &f, 0.0f,
  //                        1.0f);  // Edit 1 float using a slider from 0.0f
  //                        to 1.0f
  //     ImGui::ColorEdit3(
  //         "clear color",
  //         (float*)&clear_color);  // Edit 3 floats representing a color

  //     if (ImGui::Button("Button"))  // Buttons return true when clicked (most
  //                                   // widgets return true when
  //                                   edited/activated)
  //       counter++;
  //     ImGui::SameLine();
  //     ImGui::Text("counter = %d", counter);

  //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
  //                 1000.0f / io.Framerate, io.Framerate);
  //     ImGui::End();
  //   }

  //   // 3. Show another simple window.
  //   // if (show_another_window)
  //   // {
  //   //     ImGui::Begin("Another Window", &show_another_window);   // Pass a
  //   //     pointer to our bool variable (the window will have a closing
  //   button
  //   //     that will clear the bool when clicked) ImGui::Text("Hello from
  //   another
  //   //     window!"); if (ImGui::Button("Close Me"))
  //   //         show_another_window = false;
  //   //     ImGui::End();
  //   // }

  //   // Rendering
  //   ImGui::Render();
  //   SDL_SetRenderScale(renderer_, io.DisplayFramebufferScale.x,
  //                      io.DisplayFramebufferScale.y);
  //   SDL_SetRenderDrawColorFloat(renderer_, clear_color.x, clear_color.y,
  //                               clear_color.z, clear_color.w);
  //   SDL_RenderClear(renderer_);
  //   ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer_);
  //   SDL_RenderPresent(renderer_);
}

auto Application::delta_time() -> void {
  static auto previous_time = std::chrono::steady_clock::now();
  const auto current_time = std::chrono::steady_clock::now();
  delta_time_ =
      std::chrono::duration<float>(current_time - previous_time).count();
  previous_time = current_time;
}

}  // namespace holodeckx
