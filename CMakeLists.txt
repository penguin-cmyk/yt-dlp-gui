cmake_minimum_required(VERSION 4.0)
project(yt_dlp_downloader)

set(CMAKE_CXX_STANDARD 20)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/dependencies/imgui)
set(GLFW_DIR  ${CMAKE_SOURCE_DIR}/dependencies/GLFW)

set(GUI_DIR   ${CMAKE_SOURCE_DIR}/src/gui)

set(SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp

        ${IMGUI_DIR}/backend/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backend/imgui_impl_opengl3.cpp

        ${GUI_DIR}/gui.cpp
        src/utils/utils.h
)
add_library(imgui STATIC ${SOURCES})

target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backend
        ${GLFW_DIR}
        ${CMAKE_SOURCE_DIR}/dependencies/ImGuiFileDialog
)

add_executable(src_files
        src/main.cpp
        src/style.h
        dependencies/ImGuiFileDialog/ImGuiFileDialog.cpp
        dependencies/ImGuiFileDialog/ImGuiFileDialog.h
        dependencies/ImGuiFileDialog/ImGuiFileDialogConfig.h)

target_include_directories(src_files PRIVATE
        ${GLFW_DIR}
)

target_link_options(src_files PRIVATE
        -static
        -static-libgcc
        -static-libstdc++
)

target_link_libraries(src_files PRIVATE
        imgui
        ${GLFW_DIR}/libglfw3.a
        -Wl,-Bstatic
        -lstdc++
        -lpthread
        -Wl,-Bdynamic
        opengl32
        gdi32
        user32
)