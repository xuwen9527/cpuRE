#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_glfw.h"
#include "external/imgui/imgui_impl_opengl3.h"
#include "external/ifd/ImFileDialog.h"
#include "app.h"
#include "timer.h"

namespace cpuRE {
  App::App() : window_(nullptr), total_time_(0.0), frame_num_(0) {
  }

  App::~App() {

  }

  void App::setupFonts() {
    {
      ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
        GLuint tex;

        glGenTextures(1, &tex);
        glBindTexture(GL_TEXTURE_2D, tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_RGBA : GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);

        return (void*)(size_t)tex;
      };
      ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
        GLuint texID = (GLuint)((uintptr_t)tex);
        glDeleteTextures(1, &texID);
      };
      ifd::FileDialog::Instance().SetZoom(1.f);
    }

    glfwSetErrorCallback([](int error, const char* description) {
      std::cout << "Glfw Error " << error << " " << description;
    });

    // {
    //   static std::string defaultFontPath = "msyh.ttc";
    //   static float defaultFontSize = 20.f;

    //   auto font = ImGui::GetIO().Fonts->AddFontFromFileTTF(
    //     defaultFontPath.c_str(), defaultFontSize, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesChineseFull());

    //   if (font) {
    //     ImGui::GetIO().FontDefault = font;
    //   }
    // }

    // {
    //   static std::string fa_solid_file_name = "../fonts/fa-solid-900.ttf";
    //   static std::string fa_regular_file_name = "../fonts/fa-regular-400.ttf";
    //   static float iconFontSize = 24.f;
    //   static const ImWchar icons_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };

    //   ImFontConfig fa_solid_config;
    //   fa_solid_config.MergeMode = true;
    //   fa_solid_config.PixelSnapH = true;

    //   ImFontConfig fa_regular_config;
    //   fa_regular_config.MergeMode = true;
    //   fa_regular_config.PixelSnapH = true;
    //   ImGui::GetIO().Fonts->AddFontFromFileTTF(fa_solid_file_name.c_str(), iconFontSize, &fa_solid_config, icons_ranges);
    //   ImGui::GetIO().Fonts->AddFontFromFileTTF(fa_regular_file_name.c_str(), iconFontSize, &fa_regular_config, icons_ranges);
    // }
  }

  void App::setup() {
    glfwSetErrorCallback([](int error, const char* description) {
      std::cout << "Glfw Error " << error << " " << description;
    });

    if (!glfwInit()) {
      std::cout << "glfwInit is error." << std::endl;
      return;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_MAXIMIZED, true);
    glfwWindowHint(GLFW_DECORATED, true);
    glfwWindowHint(GLFW_RESIZABLE, true);
    glfwWindowHint(GLFW_REFRESH_RATE, 60);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    window_ = glfwCreateWindow(800, 600, "cpuRE", nullptr, nullptr);
    if (!window_) {
      std::cout << "failed to create GLFW window" << std::endl;
      return;
    }

    glfwMakeContextCurrent(window_);
    // // Enable vsync
    // glfwSwapInterval(1);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cout << "failed to load glad" << std::endl;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowPadding = ImVec2(8, 8);
    style.WindowBorderSize = 1;
    style.WindowMenuButtonPosition = ImGuiDir_None;
    style.FrameBorderSize = 1;
    style.FrameRounding = 4;

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigDockingAlwaysTabBar = true;
    io.FontAllowUserScaling = true;

    setupFonts();

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init();
  }

  void App::render() {
    const ImGuiViewport* mainviewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(mainviewport->WorkPos);
    ImGui::SetNextWindowSize(mainviewport->WorkSize);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoBringToFrontOnFocus
      | ImGuiWindowFlags_NoNavFocus
      | ImGuiWindowFlags_NoDocking
      | ImGuiWindowFlags_NoTitleBar
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoBackground
      | ImGuiWindowFlags_NoSavedSettings
      | ImGuiWindowFlags_NoBringToFrontOnFocus;

    if (ImGui::Begin("##frame", 0, window_flags)) {
      //if (ImGui::BeginChild("canvas", ImVec2(0, 0), false)) {
        // if (ImGui::IsItemActive()) {
          ImGuiIO& io = ImGui::GetIO();
          if (io.MouseWheel != 0.f) {
            renderer_.manipulator()->mouseScroll(io.MouseWheel < 0.f);
          } else if (ImGui::IsMouseDragging(ImGuiPopupFlags_MouseButtonLeft)) {
            renderer_.manipulator()->mouseMove(io.MousePos.x, io.MousePos.y);
          } else if (ImGui::IsMouseDown(ImGuiPopupFlags_MouseButtonLeft)) {
            renderer_.manipulator()->mousePress(io.MousePos.x, io.MousePos.y);
          } else if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonLeft)) {
            renderer_.manipulator()->mouseRelease(io.MousePos.x, io.MousePos.y);
          }
        // }
      //}
      //ImGui::EndChild();

      renderer_.manipulator()->apply();

      {
        Timer::instance().start();

        renderer_.render();

        total_time_ += Timer::instance().time_s();
        ++frame_num_;
      }

      auto width = renderer_.camera()->viewport().z;
      auto height = renderer_.camera()->viewport().w;

      auto preview_origin = ImGui::GetCursorScreenPos();
      auto preview_size   = ImGui::GetContentRegionAvail();
      auto preview_corner = ImVec2(preview_origin.x + preview_size.x, preview_origin.y + preview_size.y);

      auto margin = ImVec2((preview_size.x - width) / 2.0f, (preview_size.y - height) / 2.0f);
      auto image_origin = ImVec2(preview_origin.x + margin.x, preview_origin.y + margin.y);
      auto image_corner = ImVec2(image_origin.x + width, image_origin.y + height);

      ImDrawList* draw_list = ImGui::GetWindowDrawList();
      draw_list->AddImage((ImTextureID)(size_t)renderer_.framebuffer().colorTextureId(),
                          image_origin, image_corner, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));

      draw_list->AddRect(image_origin, image_corner, ImGui::GetColorU32(ImGuiCol_Border));
    }
    ImGui::End();

    ImGui::PopStyleVar(3);
  }

  void App::run() {
    setup();

    while (!glfwWindowShouldClose(window_)) {
      glfwPollEvents();

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();
      ImGui::NewFrame();

      int width, height;
      glfwGetFramebufferSize(window_, &width, &height);
      glViewport(0, 0, width, height);

      glClearColor(0.f, 0.f, 0.f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      // glEnable(GL_CULL_FACE);
      // glEnable(GL_DEPTH_TEST);
      // glEnable(GL_BLEND);
      // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

      render();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glfwSwapBuffers(window_);

      std::ostringstream stream;
      stream << std::fixed << "cpuRE FPS : " << (total_time_ ? frame_num_ / total_time_  : 0.0);
      glfwSetWindowTitle(window_, stream.str().c_str());
    }

    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(window_);
    glfwTerminate();
  }
}