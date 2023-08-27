// Copyright dummy
#ifndef LVEWINDOW_INCLUDED_H
#define LVEWINDOW_INCLUDED_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace lve {

class LveWindow {
 public:
    LveWindow(int w, int h, std::string const& name);
    LveWindow(LveWindow const&) = delete;
    LveWindow& operator=(LveWindow const&) = delete;
    ~LveWindow();

    bool shouldClose() const { return glfwWindowShouldClose(window); }
    void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
    VkExtent2D getExtent() const {
        return {static_cast<uint32_t>(width),
                static_cast<uint32_t>(height)};
    }

    bool wasWindowResized() const { return framebufferResized; }
    void resetWindowResized() { framebufferResized = false; }

 private:
    static void framebufferResizedCallback(GLFWwindow* window,
                                           int width, int height);
    void initWindow();

    int width;
    int height;
    bool framebufferResized = false;

    std::string windowName;
    GLFWwindow* window;
};  // class LveWindow

}  // namespace lve

#endif  // LVEWINDOW_INCLUDED_H
