//
// Created by sigma on 12/11/2025.
//

#ifndef IMGUI_LUA_GUI_H
#define IMGUI_LUA_GUI_H

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace gui
{
    inline float WIDTH = 800.0f;
    inline float HEIGHT = 600.0f;

    inline GLFWwindow* window = nullptr;
    inline const char* glsl_version = nullptr;

    inline bool isRunning = true;

    void CreateWindow(const char* windowName, float WIDTH, float HEIGHT, bool on_top=true) noexcept;
    void DestroyWindow() noexcept;

    void CreateImGui() noexcept;
    void DestroyImGui() noexcept;

    void BeginRender() noexcept;
    void EndRender() noexcept;

}

#endif //IMGUI_LUA_GUI_H