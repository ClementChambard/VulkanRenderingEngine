// Copyright dummy
#ifndef FIRSTAPP_INCLUDED_H
#define FIRSTAPP_INCLUDED_H

#include "./lve_window.hpp"
#include "./lve_renderer.hpp"
#include "./lve_game_object.hpp"

#include <vector>
#include <memory>

namespace lve {

class FirstApp {
 public:
    static constexpr int WIDTH = 800;
    static constexpr int HEIGHT = 600;

    FirstApp();
    ~FirstApp();
    FirstApp(FirstApp const&) = delete;
    FirstApp& operator=(FirstApp const&) = delete;

    void run();

 private:
    void loadGameObjects();

    LveWindow lveWindow{WIDTH, HEIGHT, "Hello Vulkan!"};
    LveDevice lveDevice{lveWindow};
    LveRenderer lveRenderer{lveWindow, lveDevice};
    std::vector<LveGameObject> gameObjects;
};  // class FirstApp

}  // namespace lve

#endif  // FIRSTAPP_INCLUDED_H
