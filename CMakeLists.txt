cmake_minimum_required(VERSION 3.12)
project(PixelSandbox)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(pixelsandbox
    main.cpp
    gui.cpp
    imgui/imgui.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/backends/imgui_impl_glfw.cpp
    imgui/backends/imgui_impl_opengl3.cpp
)

target_include_directories(pixelsandbox PRIVATE
    imgui
    imgui/backends
)

if(WIN32)
    target_link_libraries(pixelsandbox PRIVATE glfw3 opengl32 gdi32 imm32)
else()
    find_package(OpenGL REQUIRED)
    find_package(glfw3 REQUIRED)
    target_link_libraries(pixelsandbox PRIVATE glfw OpenGL::GL dl pthread)
endif()

