#include "gui.h"
#include "imgui.h"
#include <iostream>
#include <string>

void GameUI::init(GLFWwindow* window) {
    std::cout << "UI initialized\n";
    // Здесь обычно инициализируется контекст ImGui для GLFW и Vulkan
}

void GameUI::render() {
    // Базовые флаги для создания панелей без лишних рамок и заголовков
    ImGuiWindowFlags flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove;

    // 1. Верхняя панель (Кнопки 1-6 и A-F)
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 45));
    ImGui::Begin("TopBar", nullptr, flags | ImGuiWindowFlags_NoScrollbar);
    
    for (int i = 1; i <= 6; ++i) {
        ImGui::Button(std::to_string(i).c_str(), ImVec2(35, 30));
        if (i < 6) ImGui::SameLine();
    }
    
    // Сдвигаем правую группу кнопок в конец экрана
    ImGui::SameLine(ImGui::GetWindowWidth() - 240);
    
    const char* letters[] = {"A", "B", "C", "D", "E", "F"};
    for (int i = 0; i < 6; ++i) {
        ImGui::Button(letters[i], ImVec2(35, 30));
        if (i < 5) ImGui::SameLine();
    }
    ImGui::End();

    // 2. Левая панель "Управление"
    ImGui::SetNextWindowPos(ImVec2(10, 60));
    ImGui::Begin("ControlsLeft", nullptr, flags | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Управление");
    ImGui::Dummy(ImVec2(0, 2));
    
    if (ImGui::Button("  ^  ", ImVec2(65, 30))) {}
    if (ImGui::Button(" < ", ImVec2(30, 30))) { ImGui::SameLine(); }
    if (ImGui::Button(" v ", ImVec2(30, 30))) {}
    if (ImGui::Button(" > ", ImVec2(30, 30))) {}
    
    ImGui::Dummy(ImVec2(0, 2));
    ImGui::Button("TAB", ImVec2(65, 30));
    ImGui::Button("DEL", ImVec2(65, 30));
    ImGui::Button("ESC", ImVec2(65, 30));
    ImGui::End();

    // 3. Правая панель "Клавиши"
    ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 120, 60));
    ImGui::Begin("ControlsRight", nullptr, flags | ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Клавиши");
    ImGui::Dummy(ImVec2(0, 2));
    
    ImGui::Button("ЛКМ", ImVec2(45, 30)); ImGui::SameLine();
    ImGui::Button("ПКМ", ImVec2(45, 30));
    ImGui::Button("A-F", ImVec2(45, 30)); ImGui::SameLine();
    ImGui::Button(" / ", ImVec2(45, 30));
    
    ImGui::Button("MRB", ImVec2(94, 30));
    ImGui::Button("BKSP", ImVec2(94, 30));
    ImGui::Button("SPACE", ImVec2(94, 30));
    ImGui::Button("ENTER", ImVec2(94, 30));
    ImGui::End();

    // 4. Нижняя панель инструментов (сетка элементов и материалов)
    ImGui::SetNextWindowPos(ImVec2(100, ImGui::GetIO().DisplaySize.y - 130));
    ImGui::Begin("ToolbarBottom", nullptr, flags | ImGuiWindowFlags_AlwaysAutoResize);
    
    // Первый ряд (инструменты)
    for (int i = 0; i < 8; ++i) {
        ImGui::PushID(i);
        ImGui::Button("T", ImVec2(32, 32));
        if (i < 7) ImGui::SameLine();
        ImGui::PopID();
    }
    
    // Второй и третий ряды (материалы)
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 12; ++col) {
            ImGui::PushID(row * 100 + col);
            ImGui::Button("M", ImVec2(32, 32));
            if (col < 11) ImGui::SameLine();
            ImGui::PopID();
        }
    }
    
    ImGui::End();
}

void GameUI::cleanup() {
    std::cout << "UI cleaned up\n";
}

