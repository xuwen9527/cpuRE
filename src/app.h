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

    void run();

  private:
    void setup();

    void render();

    GLFWwindow* window_;

    Display display_;
  };
}

#endif