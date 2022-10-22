#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <memory>
#include <glm/glm.hpp>

namespace cpuRE {
  class FrameBuffer {
	public:
    FrameBuffer();

    void resize(int width, int height);

    const auto& colorBuffer() { return color_buffer_; }
    const auto& depthBuffer() { return depth_buffer_; }

    auto colorTextureId() { return color_texture_id_; }
    auto depthTextureId() { return depth_texture_id_; }

    const auto& clearColor() { return clear_color_; }
    void clearColor(const glm::vec4& clear_color);

    auto clearDepth() { return clear_depth_; }
    void clearDepth(const float clear_depth);

    void updateTexture();

  private:
    glm::ivec2 size_;
    std::unique_ptr<unsigned char> color_buffer_;
    std::unique_ptr<unsigned short> depth_buffer_;

    unsigned int color_texture_id_;
    unsigned int depth_texture_id_;

    glm::vec4 clear_color_;
    float clear_depth_;
	};
}

#endif