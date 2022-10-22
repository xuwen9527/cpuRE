#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "renderer.h"

namespace cpuRE {
  class Display {
  public:
    Display();

    void render();

    double fps();

  private:
    void manipulate();

    void frame();

    void drawFramebuffer();

    Renderer renderer_;

    double total_time_;
    unsigned int frame_num_;
  };
}

#endif