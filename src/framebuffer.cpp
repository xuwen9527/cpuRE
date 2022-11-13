#include <glad/glad.h>
#include "framebuffer.h"
#include "pipeline/buffer_io.h"

namespace {
  template <typename T>
  void alloc_buffer(const glm::ivec2& size, int components, std::unique_ptr<T>& ptr) {
    ptr.reset(new T[components * size.x * size.y]);
    memset(ptr.get(), 0, sizeof(T) * components * size.x * size.y);
  }
}

namespace cpuRE {
  FrameBuffer::FrameBuffer() : size_(0, 0), color_texture_id_(0), depth_texture_id_(0),
   clear_color_(0.45f, 0.55f, 0.60f, 1.0f), clear_depth_(1.f) {
  }

  FrameBuffer::~FrameBuffer() {
    if (color_texture_id_) {
      glDeleteTextures(1, &color_texture_id_);
    }

    if (depth_texture_id_) {
      glDeleteTextures(1, &depth_texture_id_);
    }
  }

  void FrameBuffer::resize(const glm::ivec2& size) {
    if (size.x < 2 || size.y < 2) {
      return;
    }

    if (size_ != size) {
      size_ = size;
      alloc_buffer<uint8_t>(size_, 4, color_buffer_);
      alloc_buffer<uint16_t>(size_, 1, depth_buffer_);
    }

    updateTexture();
  }

  void FrameBuffer::apply() {
    for (auto x = 0; x < size_.x; ++x) {
      for (auto y = 0; y < size_.y; ++y) {
        BufferIO::writeColor(x, y, clear_color_, color_buffer_.get(), size_.x);
    }
  }

    for (auto x = 0; x < size_.x; ++x) {
      for (auto y = 0; y < size_.y; ++y) {
        BufferIO::writeDepth(x, y, clear_depth_, depth_buffer_.get(), size_.x);
      }
    }
  }

  void FrameBuffer::updateTexture() {
    if (!color_texture_id_) {
      glGenTextures(1, &color_texture_id_);
    }

    if (!depth_texture_id_) {
      glGenTextures(1, &depth_texture_id_);
    }

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    {
      glBindTexture(GL_TEXTURE_2D, color_texture_id_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size_.x, size_.y, 0,
                   GL_RGBA, GL_UNSIGNED_BYTE, color_buffer_.get());
    }

    {
      glBindTexture(GL_TEXTURE_2D, depth_texture_id_);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, size_.x, size_.y, 0,
                   GL_RED, GL_UNSIGNED_SHORT, depth_buffer_.get());
    }

    glBindTexture(GL_TEXTURE_2D, 0);
  }
}