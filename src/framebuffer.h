#ifndef __FRAMEBUFFER_H__
#define __FRAMEBUFFER_H__

#include <memory>
#include <glm/glm.hpp>

namespace cpuRE {
  class FrameBuffer {
	public:
    FrameBuffer();
    ~FrameBuffer();

    void resize(const glm::ivec2& size);
    const glm::ivec2& size() const { return size_; }

    const auto& colorBuffer() const { return color_buffer_; }
    const auto& depthBuffer() const { return depth_buffer_; }

    auto colorTextureId() { return color_texture_id_; }
    auto depthTextureId() { return depth_texture_id_; }

    auto& clearColor() { return clear_color_; }
    auto& clearDepth() { return clear_depth_; }

    void apply();
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