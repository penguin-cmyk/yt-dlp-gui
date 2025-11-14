//
// Created by sigma on 12/11/2025.
//

#include "gui.h"

namespace {
    bool isDragging = false;
    double lastMouseX = 0.0;
    double lastMouseY = 0.0;
    const float DRAG_AREA_HEIGHT = 30.0f;
}

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << description << std::endl;
}

static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            double xpos, ypos;
            glfwGetCursorPos(window, &xpos, &ypos);

            if (ypos <= DRAG_AREA_HEIGHT)
            {
                isDragging = true;
                lastMouseX = xpos;
                lastMouseY = ypos;
            }
        }
        else if (action == GLFW_RELEASE)
        {
            isDragging = false;
        }
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (isDragging)
    {
        int windowX, windowY;
        glfwGetWindowPos(window, &windowX, &windowY);

        double deltaX = xpos - lastMouseX;
        double deltaY = ypos - lastMouseY;

        glfwSetWindowPos(window, windowX + (int)deltaX, windowY + (int)deltaY);
    }
}

void gui::CreateWindow(const char *windowName, float WIDTH, float HEIGHT, bool on_top) noexcept
{
    glfwSetErrorCallback(glfw_error_callback);

    if (!glfwInit())
        return;

    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Borderless window
    glfwWindowHint(GLFW_FLOATING, on_top); // Always on top

    window = glfwCreateWindow(static_cast<int>(WIDTH), static_cast<int>(HEIGHT), windowName, nullptr, nullptr);
    if (window == nullptr)
        return;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vysnc

    //glfwSetWindowOpacity(window, 0.99);

    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    gui::glsl_version = glsl_version;
}

void gui::DestroyWindow() noexcept
{
    if (window)
    {
        glfwDestroyWindow(window);
        window = nullptr;
    }

    glfwTerminate();
}

void gui::CreateImGui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = nullptr;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(gui::glsl_version);
}

void gui::DestroyImGui() noexcept
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void gui::BeginRender() noexcept
{
    glfwPollEvents();

    if (glfwWindowShouldClose(window))
    {
        gui::isRunning = false;
        return;
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void gui::EndRender() noexcept
{
    ImGui::EndFrame();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}


