#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "external/imgui/imgui.h"
#include "external/imgui/imgui_impl_glfw.h"
#include "external/imgui/imgui_impl_opengl3.h"
#include "app.h"

namespace cpuRE {
  App::App() : window_(nullptr) {
  }

  App::~App() {

  }

  void App::setup() {
    glfwSetErrorCallback([](int error, const char* description) {
      std::cout << "glfw Error " << error << " " << description;
    });

    if (!glfwInit()) {
      std::cout << "glfwInit is error." << std::endl;
      return;
    }

    glfwDefaultWindowHints();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Required on Mac
#endif
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
    io.FontAllowUserScaling = true;
    io.FontGlobalScale = 2.f;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigDockingAlwaysTabBar = true;

    ImGui_ImplGlfw_InitForOpenGL(window_, true);
    ImGui_ImplOpenGL3_Init();
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

      display_.render();

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      glfwSwapBuffers(window_);

      std::ostringstream stream;
      stream << std::fixed << std::setprecision(2) << "cpuRE FPS : " << display_.fps();
      glfwSetWindowTitle(window_, stream.str().c_str());
    }

    ImGui_ImplGlfw_Shutdown();
    glfwDestroyWindow(window_);
    glfwTerminate();
  }
}