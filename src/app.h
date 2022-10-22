#ifndef __APP_H__
#define __APP_H__

#include <memory>
#include "display.h"

struct GLFWwindow;

namespace cpuRE {
  class App {
  public:
    App();
    ~App();

    inline GLFWwindow* window() { return window_; }

    void run();

  protected:
    void setup();
    void setupFonts();

    void render();

  private:
    GLFWwindow* window_;

    Display display_;
  };
}

#endif