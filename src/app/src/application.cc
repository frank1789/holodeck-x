#include "application.hh"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <fmt/format.h>
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <imgui_internal.h>
// #include "renderer.hh"

#include <backward.hpp>
#include <chrono>
#include <memory>
#include <stdexcept>

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

  const float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  SDL_WindowFlags window_flags =
      SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN | SDL_WINDOW_HIGH_PIXEL_DENSITY;
  const auto width = static_cast<int>(1280 * main_scale);
  const auto height = static_cast<int>(800 * main_scale);
  window_ = SDL_CreateWindow("CalculiX", width, height, window_flags);
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
  ImGuiIO &io = ImGui::GetIO();
  (void)io;
  io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
  io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
  io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  // ImGui::StyleColorsLight();

  // Setup scaling
  float main_scale = SDL_GetDisplayContentScale(SDL_GetPrimaryDisplay());
  ImGuiStyle &style = ImGui::GetStyle();
  style.ScaleAllSizes(
      main_scale); // Bake a fixed style scale. (until we have a solution for
                   // dynamic style scaling, changing this requires resetting
                   // Style + calling this again)
  style.FontScaleDpi =
      main_scale; // Set initial font scale. (in docking branch: using
                  // io.ConfigDpiScaleFonts=true automatically overrides this
                  // for every window depending on the current monitor)

  // Setup Platform/Renderer backends
  ImGui_ImplSDL3_InitForSDLRenderer(window_, renderer_);
  ImGui_ImplSDLRenderer3_Init(renderer_);

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
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf");
  // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf");
  // ImFont* font =
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf");
  // IM_ASSERT(font != nullptr);

  // Our state
  bool show_demo_window = true;
  bool show_another_window = false;
  ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

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
        if (event.type == SDL_EVENT_QUIT)
          running_ = false;
        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED &&
            event.window.windowID == SDL_GetWindowID(window_))
          running_ = false;
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
      ImGuiIO &io = ImGui::GetIO();
      if (io.DisplaySize.x <= 0.0f || io.DisplaySize.y <= 0.0f) {
        int w, h;
        SDL_GetWindowSize(window_, &w, &h);
        // If SDL reports 0, force it to 1280x720 (or your starting size) so
        // ImGui won't assert
        io.DisplaySize =
            ImVec2(w > 0 ? (float)w : 1280.0f, h > 0 ? (float)h : 720.0f);
      }
      ImGui::NewFrame(); // 3rd: Core ImGui frame initialization (Now safe!)
      ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

      // --- 4. Your UI Code (Demo and Simple Windows) ---
      // 1. Show the big demo window (Most of the sample code is in
      // ImGui::ShowDemoWindow()! You can browse its code to learn more about
      // Dear ImGui!).
      // if (show_demo_window)
      //   ImGui::ShowDemoWindow(&show_demo_window);


      ImGuiID dockspace_id = ImGui::GetID("My Dockspace");
      // ImGuiViewport* viewport = ImGui::GetMainViewport();
      // if(ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
      // {
      //   ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
      //   ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);
      //   ImGuiID dock_id_left = 0;
      //   ImGuiID dock_id_main =dockspace_id;
      //   ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.20f, &dock_id_left, &dock_id_main);
      //   ImGuiID dock_id_left_top = 0;
      //   ImGuiID dock_id_left_bottom = 0;
      //   ImGui::DockBuilderSplitNode(dock_id_left, ImGuiDir_Up, 0.50f, &dock_id_left_top, &dock_id_left_bottom);
      //   ImGui::DockBuilderDockWindow("Main", dock_id_main);
      //   ImGui::DockBuilderDockWindow("Properties", dock_id_left_top);
      //   ImGui::DockBuilderDockWindow("Scene", dock_id_left_bottom);
      //   ImGui::DockBuilderDockWindow("Main", dock_id_main);
      //   ImGui::DockBuilderFinish(dockspace_id);
      // }
      // ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);
      // ImGui::Begin("Properties");

      /////
      //ImGui::DockBuilderDockWindow("Main", dock_id_main);
//       #include "imgui.h"
// #include "imgui_internal.h" // Required for the advanced DockBuilder API

// void RenderUI()
// {
    // 1. Setup a Fullscreen Dockspace
    // This allows the docking system to fill the entire application workspace.
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    // Window style overrides to make it seamless
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    // Hidden window flags that turn this window into an invisible background hub
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
    window_flags |= ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    // Submit the parent window
    bool open = true;
    ImGui::Begin("MainDockSpaceWindow", &open, window_flags);
    ImGui::PopStyleVar(3);

    // Submit the actual DockSpace
    //ImGuiID dockspace_id = ImGui::GetID("MyApplicationDockspace");
    ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

    // 2. Programmatically Define the Default Layout
    // The conditional check ensures we only enforce this layout the VERY first time the app runs.
    // Afterwards, ImGui loads the user's custom changes from the imgui.ini file.
    if (ImGui::DockBuilderGetNode(dockspace_id) == nullptr)
    {
        // Clear any existing layout structures
        ImGui::DockBuilderRemoveNode(dockspace_id);
        ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace);
        ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->WorkSize);

        // Split the central space into compartments
        ImGuiID dock_id_main = dockspace_id;
        ImGuiID dock_id_left;
        ImGuiID dock_id_bottom;

        // Split Left: takes 25% of the width from the total main node
        ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Left, 0.25f, &dock_id_left, &dock_id_main);

        // Split Bottom: takes 30% of the height from the REMAINING main node
        ImGui::DockBuilderSplitNode(dock_id_main, ImGuiDir_Down, 0.30f, &dock_id_bottom, &dock_id_main);

        // Map your window titles onto the generated Dock IDs
        // NOTE: These strings MUST match the window names declared in ImGui::Begin() below.
        ImGui::DockBuilderDockWindow("Inspector", dock_id_left);
        ImGui::DockBuilderDockWindow("Console Output", dock_id_bottom);
        ImGui::DockBuilderDockWindow("Viewport Canvas", dock_id_main);

        ImGui::DockBuilderFinish(dockspace_id);
        ImGui::DockSpaceOverViewport(dockspace_id, viewport, ImGuiDockNodeFlags_PassthruCentralNode);
        ImGui::End();
    }

    // Optional: Add a Global Menu Bar over your dockspace
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Exit")) { /* Handle exit logic */ }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End(); // End of MainDockSpaceWindow

    // 3. Render the Component Windows
    // ImGui will catch these by name and automatically snap them into position.

    ImGui::Begin("Inspector");
    ImGui::Text("Entity Properties");
    ImGui::End();

    ImGui::Begin("Console Output");
    //ImGui::Text("System Log: All systems nominal.");
        // 1. Create the Tab Bar container
    if (ImGui::BeginTabBar("MyViewportTabBar"))
    {
        // 2. Define the first tab
        if (ImGui::BeginTabItem("Viewport 1"))
        {
            ImGui::Text("Render target or scene view goes here.");

            ImGui::EndTabItem(); // Must pair with BeginTabItem
        }

        // 3. Define the second tab
        if (ImGui::BeginTabItem("Viewport 2"))
        {
            ImGui::Text("Alternative camera view or settings.");

            ImGui::EndTabItem(); // Must pair with BeginTabItem
        }

        ImGui::EndTabBar(); // Must pair with BeginTabBar
    }
    ImGui::End();

    ImGui::Begin("Viewport Canvas");
    ImGui::Text("Your 3D/2D Engine Scene Renders Here");
    ImGui::End();



      // 2. Show a simple window that we create ourselves. We use a Begin/End
      // pair to create a named window.
      // {
      //   static float f = 0.0f;
      //   static int counter = 0;

      //   if (ImGui::Begin("Hello, world!")) {
      //     ImGui::Text("This is some useful text.");
      //     ImGui::Checkbox("Demo Window", &show_demo_window);
      //     ImGui::Checkbox("Another Window", &show_another_window);

      //     ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
      //     ImGui::ColorEdit3("clear color", (float *)&clear_color);

      //     if (ImGui::Button("Button")) {
      //       counter++;
      //     }
      //     ImGui::SameLine();
      //     ImGui::Text("counter = %d", counter);

      //     ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
      //                 1000.0f / io.Framerate, io.Framerate);
      //   }
      //   ImGui::End();
      // }

      // // 3. Show another simple window.
      // if (show_another_window) {
      //   ImGui::Begin(
      //       "Another Window",
      //       &show_another_window); // Pass a pointer to our bool variable (the
      //                              // window will have a closing button that
      //                              // will clear the bool when clicked)
      //   ImGui::Text("Hello from another window!");
      //   if (ImGui::Button("Close Me"))
      //     show_another_window = false;
      //   ImGui::End();
      // }

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
