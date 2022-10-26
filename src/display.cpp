#include <glm/glm.hpp>
#include "external/imgui/imgui.h"
#include "display.h"
#include "timer.h"

namespace cpuRE {
  Display::Display() : total_time_(0.0), frame_num_(0) {

  }

  void Display::showOptions() {
    if (ImGui::Begin("Render Options", 0)) {

      ImGui::Text("Viewport");
      ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::Separator();
      if (ImGui::BeginTable("##viewport", 2, ImGuiTableFlags_SizingStretchProp)) {
        static int x, y, width, height;
        auto viewport = renderer_.camera()->viewport();
        x = viewport.x;
        y = viewport.y;
        width = viewport.z;
        height = viewport.w;

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("X");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          if (ImGui::DragInt("##viewport_x", &x, 1, 0, 5000)) {
            renderer_.camera()->viewport(x, viewport.y, viewport.z, viewport.w);
          }
        }

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Y");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          if (ImGui::DragInt("##viewport_y", &y, 1, 0, 5000)) {
            renderer_.camera()->viewport(viewport.x, y, viewport.z, viewport.w);
          }
        }

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Width");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          if (ImGui::DragInt("##viewport_width", &width, 1, 2, 10000)) {
            renderer_.camera()->viewport(viewport.x, viewport.y, width, viewport.w);
          }
        }

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Height");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          if (ImGui::DragInt("##viewport_height", &height, 1, 2, 10000)) {
            renderer_.camera()->viewport(viewport.x, viewport.y, viewport.z, height);
          }
        }

        ImGui::EndTable();
      }
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::NewLine();

      ImGui::Text("Draw Options");
      ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
      {
        ImGui::Separator();
        if (ImGui::BeginTable("##draw_options", 2, ImGuiTableFlags_SizingStretchProp)) {
          auto& options = renderer_.options();

          ImGui::TableNextRow();
          {
            ImGui::TableSetColumnIndex(0);
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::Checkbox("draw bin", &options.draw_bin);
          }

          ImGui::TableNextRow();
          {
            ImGui::TableSetColumnIndex(0);
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::Checkbox("draw tile", &options.draw_tile);
          }

          ImGui::TableNextRow();
          {
            ImGui::TableSetColumnIndex(0);
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::Checkbox("draw stamp", &options.draw_stamp);
          }

          ImGui::TableNextRow();
          {
            ImGui::TableSetColumnIndex(0);
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::Checkbox("draw edge", &options.draw_edge);
          }

          ImGui::EndTable();
        }
      }
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::NewLine();
    }
    ImGui::End();
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

    showOptions();
  }

  double Display::fps() {
    return total_time_ ? frame_num_ / total_time_  : 0.0;
  }

  void Display::manipulate() {
    if (ImGui::IsWindowHovered()) {
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
    }

    renderer_.manipulator()->apply();
  }

  void Display::frame() {
    Timer::instance().start();

    renderer_.render();

    if (frame_num_ > 30) {
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
    auto image_corner = ImVec2(image_origin.x + width + 1, image_origin.y + height + 1);

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    draw_list->AddImage((ImTextureID)(size_t)renderer_.framebuffer().colorTextureId(),
                        image_origin, image_corner, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));

    draw_list->AddRect(image_origin, image_corner, ImGui::GetColorU32(ImGuiCol_Border));
  }
}