#include <glm/glm.hpp>
#include "external/imgui/imgui.h"
#include "display.h"
#include "timer.h"

namespace cpuRE {
  Display::Display() : total_time_(0.0), frame_num_(0), framebuffer_scale_(1),
    framebuffer_index_(0) {

  }

  void Display::render() {
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.f);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoNavFocus
      | ImGuiWindowFlags_NoCollapse
      | ImGuiWindowFlags_NoResize
      | ImGuiWindowFlags_NoMove
      | ImGuiWindowFlags_NoSavedSettings;

    ImGui::SetNextWindowDockID(ImGui::GetID("DockSpace"));
    if (ImGui::Begin("Preview", 0, window_flags)) {
      manipulate();
      frame();
      drawFramebuffer();
    }
    ImGui::End();

    ImGui::PopStyleVar(3);

    showOptions();
    showStatus();
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
      } else if (ImGui::IsKeyDown(ImGuiKey_Escape)) {

      } else if (ImGui::IsKeyDown(ImGuiKey_Space)) {
        BoundingBox box;
        box.expandBy(renderer_.geometry()->vertices);
        BoundingSphere sphere(box);
        auto vp = renderer_.manipulator()->createViewpoint(sphere);
        renderer_.manipulator()->viewpoint(vp, 2.f);
      }
    }

    renderer_.manipulator()->apply();
  }

  void Display::frame() {
    Timer timer;

    renderer_.render();

    if (frame_num_ > 30) {
      total_time_ = 0.0;
      frame_num_ = 0;
    }
    total_time_ += timer.time_s();
    ++frame_num_;
  }

  void Display::drawFramebuffer() {
    auto image_size = renderer_.framebuffer().size();
    auto width  = image_size.x * framebuffer_scale_;
    auto height = image_size.y * framebuffer_scale_;

    auto ui_origin = ImGui::GetCursorScreenPos();
    auto ui_size   = ImGui::GetContentRegionAvail();
    auto ui_corner = ImVec2(ui_origin.x + ui_size.x, ui_origin.y + ui_size.y);

    auto margin = ImVec2((ui_size.x - width + 1) / 2.0f, (ui_size.y - height + 1) / 2.0f);
    auto image_origin = ImVec2(ui_origin.x + margin.x, ui_origin.y + margin.y);
    auto image_corner = ImVec2(image_origin.x + width, image_origin.y + height);

    auto buffer = (ImTextureID)(size_t)renderer_.framebuffer().colorTextureId();

    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    if (framebuffer_index_ == 1) {
      buffer = (ImTextureID)(size_t)renderer_.framebuffer().depthTextureId();
    }
    draw_list->AddImage(buffer, image_origin, image_corner, ImVec2(0.f, 1.f), ImVec2(1.f, 0.f));
  }

  void Display::showOptions() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(800, 800));
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
          ImGui::Text("Width");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&width);
          if (ImGui::DragInt("##", &width, 1, 2, 10000)) {
            renderer_.camera()->viewport(viewport.x, viewport.y, width, viewport.w);
          }
          ImGui::PopID();
        }

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Height");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&height);
          if (ImGui::DragInt("##", &height, 1, 2, 10000)) {
            renderer_.camera()->viewport(viewport.x, viewport.y, viewport.z, height);
          }
          ImGui::PopID();
        }

        ImGui::EndTable();
      }
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::NewLine();

      ImGui::Text("Draw Options");
      ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::Separator();
      if (ImGui::BeginTable("##draw_options", 2, ImGuiTableFlags_SizingStretchProp)) {
        auto& options = renderer_.options();

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&options.draw_bin);
          ImGui::Checkbox("Draw Bin", &options.draw_bin);
          ImGui::PopID();
        }

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&options.draw_tile);
          ImGui::Checkbox("Draw Tile", &options.draw_tile);
          ImGui::PopID();
        }

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&options.draw_bounds);
          ImGui::Checkbox("Draw Bounds", &options.draw_bounds);
          ImGui::PopID();
        }

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&options.draw_stamp);
          ImGui::Checkbox("Draw Stamp", &options.draw_stamp);
          ImGui::PopID();
        }

        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&options.draw_truth);
          ImGui::Checkbox("Draw Truth", &options.draw_truth);
          ImGui::PopID();
        }

        ImGui::EndTable();
      }
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::NewLine();

      ImGui::Text("Framebuffer");
      ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::Separator();
      if (ImGui::BeginTable("##framebuffer", 2, ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Clear Color");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);

          auto ptr = glm::value_ptr(renderer_.framebuffer().clearColor());
          ImGui::PushID(ptr);
          ImGui::ColorEdit4("##clear_color", ptr, ImGuiColorEditFlags_InputRGB);
          ImGui::PopID();
        }

        ImGui::TableNextRow();
        {
          static const char* items[] = { "color", "depth" };

          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Buffer");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&framebuffer_index_);
          ImGui::Combo("##", &framebuffer_index_, items, 2);
          ImGui::PopID();
        }

        ImGui::TableNextRow();
        {
          static int current_scale = 0;
          static const char* items[] = { "1", "2", "4", "8" };

          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Scale");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&framebuffer_scale_);
          if (ImGui::Combo("##", &current_scale, items, 4)) {
            framebuffer_scale_ = pow(2, current_scale);
            static auto pan_speed = renderer_.manipulator()->panSpeed();
            renderer_.manipulator()->panSpeed(pan_speed / framebuffer_scale_);

            static auto rotate_speed = renderer_.manipulator()->rotateSpeed();
            renderer_.manipulator()->rotateSpeed(rotate_speed / framebuffer_scale_);
          }
          ImGui::PopID();
        }

        ImGui::EndTable();
      }
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::NewLine();

      ImGui::Text("Geometry");
      ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::Separator();
      if (ImGui::BeginTable("##geometry", 2, ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableNextRow();
        {
          static int current_geo = 0;
          static const char* items[] = { "Triangle", "Icosahedron" };

          ImGui::TableSetColumnIndex(0);
          ImGui::Text("Geometry");

          ImGui::TableSetColumnIndex(1);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(renderer_.geometry().get());
          if (ImGui::Combo("##", &current_geo, items, 2)) {
            if (current_geo == 0) {
              renderer_.geometry() = createTriangleGeometry();
            } else {
              renderer_.geometry() = createIcosahedronGeometry();
            }
          }
          ImGui::PopID();
        }

        ImGui::EndTable();
      }
    }
    ImGui::End();
  }

  void Display::showStatus() {
    ImGui::SetNextWindowSizeConstraints(ImVec2(200, 200), ImVec2(800, 800));
    if (ImGui::Begin("OpenGL Status", 0)) {
      ImGui::Indent(ImGui::GetTreeNodeToLabelSpacing());
      if (ImGui::BeginTable("##opengl_status", 2, ImGuiTableFlags_SizingStretchProp)) {
        ImGui::TableNextRow();
        {
          ImGui::TableSetColumnIndex(0);
          ImGui::SetNextItemWidth(-FLT_MIN);
          ImGui::PushID(&renderer_.status().cull_face);
          ImGui::Checkbox("CULL FACE", &renderer_.status().cull_face);
          ImGui::PopID();
        }

        ImGui::EndTable();
      }
      ImGui::Unindent(ImGui::GetTreeNodeToLabelSpacing());
      ImGui::NewLine();
    }
    ImGui::End();
  }
}