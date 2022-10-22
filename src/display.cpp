#include <glm/glm.hpp>
#include "external/imgui/imgui.h"
#include "display.h"
#include "timer.h"

namespace cpuRE {
  Display::Display() : total_time_(0.0), frame_num_(0) {

  }

  void Display::render() {
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(main_viewport->WorkPos);
    ImGui::SetNextWindowSize(main_viewport->WorkSize);

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
      manipulate();
      frame();
      drawFramebuffer();
    }
    ImGui::End();

    ImGui::PopStyleVar(3);
  }

  double Display::fps() {
    return total_time_ ? frame_num_ / total_time_  : 0.0;
  }

  void Display::manipulate() {
    //if (ImGui::BeginChild("canvas", ImVec2(0, 0), false)) {
      // if (ImGui::IsItemActive()) {
        ImGuiIO& io = ImGui::GetIO();
        if (io.MouseWheel != 0.f) {
          renderer_.manipulator()->mouseScroll(io.MouseWheel < 0.f);
        } else if (ImGui::IsMouseDragging(ImGuiPopupFlags_MouseButtonLeft)) {
          renderer_.manipulator()->mouseMove(io.MousePos.x, io.MousePos.y, true);
        } else if (ImGui::IsMouseDragging(ImGuiPopupFlags_MouseButtonRight)) {
          renderer_.manipulator()->mouseMove(io.MousePos.x, io.MousePos.y, false);
        } else if (ImGui::IsMouseDown(ImGuiPopupFlags_MouseButtonLeft)) {
          renderer_.manipulator()->mousePress(io.MousePos.x, io.MousePos.y);
        } else if (ImGui::IsMouseDown(ImGuiPopupFlags_MouseButtonRight)) {
          renderer_.manipulator()->mousePress(io.MousePos.x, io.MousePos.y);
        } else if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonLeft)) {
          renderer_.manipulator()->mouseRelease(io.MousePos.x, io.MousePos.y);
        } else if (ImGui::IsMouseReleased(ImGuiPopupFlags_MouseButtonRight)) {
          renderer_.manipulator()->mouseRelease(io.MousePos.x, io.MousePos.y);
        }
      // }
    //}
    //ImGui::EndChild();

    renderer_.manipulator()->apply();
  }

  void Display::frame() {
    Timer::instance().start();

    renderer_.render();

    if (frame_num_ > 10) {
      total_time_ = 0.0;
      frame_num_ = 0;
    }
    total_time_ += Timer::instance().time_s();
    ++frame_num_;
  }

  void Display::drawFramebuffer() {
    auto width = renderer_.camera()->viewport().z;
    auto height = renderer_.camera()->viewport().w;

    auto ui_origin = ImGui::GetCursorScreenPos();
    auto ui_size   = ImGui::GetContentRegionAvail();
    auto ui_corner = ImVec2(ui_origin.x + ui_size.x, ui_origin.y + ui_size.y);

    auto margin = ImVec2((ui_size.x - width) / 2.0f, (ui_size.y - height) / 2.0f);
    auto image_origin = ImVec2(ui_origin.x + margin.x, ui_origin.y + margin.y);
    auto image_corner = ImVec2(image_origin.x + width, image_origin.y + height);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddImage((ImTextureID)(size_t)renderer_.framebuffer().colorTextureId(),
                        image_origin, image_corner, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));

    draw_list->AddRect(image_origin, image_corner, ImGui::GetColorU32(ImGuiCol_Border));
  }
}