#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cstdint>

// 1. Настройки игры и редактора поля
struct GameSettings {
    int gridWidth = 120;
    int gridHeight = 160;
    int brushSize = 4;
    bool gravityEnabled = true;
    float simulationSpeed = 1.0f;
    int currentMaterialId = 1;
};

// 2. Свойства материала (с твоими цветами и иконками)
struct Material {
    int id;
    std::string name;
    std::string iconPath;
    uint32_t tabBgColor; // Цвет вкладки/фона
    uint32_t particleColor;
    bool isStatic;
};

// 3. Состояния игры (экраны)
enum class GameState {
    MAIN_MENU,
    PLAYING,
    SETTINGS,
    ABOUT
};

class SandboxApp {
private:
    GameSettings settings;
    GameState currentState;
    std::vector<Material> materials;
    std::vector<std::vector<int>> grid;
    bool isRunning;

public:
    SandboxApp() {
        currentState = GameState::MAIN_MENU;
        isRunning = true;
        initMaterials();
        resizeGrid(settings.gridWidth, settings.gridHeight);
    }

    void initMaterials() {
        materials = {
            { 1, "Sand", "icons/sand.png", 0xCCFF00, 0xE0C068, false }, // Ярко-зеленый фон
            { 2, "Water", "icons/water.png", 0x3399FF, 0x3366FF, false }, // Синий фон
            { 3, "Stone", "icons/stone.png", 0x808080, 0x606060, true }   // Серый фон
        };
    }

    // Редактор размера игрового поля
    void resizeGrid(int w, int h) {
        settings.gridWidth = std::clamp(w, 20, 500);
        settings.gridHeight = std::clamp(h, 20, 500);
        grid.assign(settings.gridHeight, std::vector<int>(settings.gridWidth, 0));
        std::cout << "[Editor] Поле изменено на: " << settings.gridWidth << "x" << settings.gridHeight << "\n";
    }

    // Обработка нажатий на кнопки главного меню
    void handleMenuClick(int buttonId) {
        switch (buttonId) {
            case 1: // Кнопка "SANDBOX" (Играть)
                currentState = GameState::PLAYING;
                std::cout << "Переход в игру...\n";
                break;
            case 2: // Кнопка "SETTINGS" (Настройки / Редактор поля)
                currentState = GameState::SETTINGS;
                std::cout << "Открытие настроек...\n";
                break;
            case 3: // Кнопка "ABOUT"
                currentState = GameState::ABOUT;
                std::cout << "Открытие 'О программе'...\n";
                break;
            case 0: // Кнопка "ВЫХОД" / Назад в меню
                currentState = GameState::MAIN_MENU;
                std::cout << "Возврат в главное меню...\n";
                break;
        }
    }

    // Главный цикл отрисовки и логики
    void update() {
        switch (currentState) {
            case GameState::MAIN_MENU:
                // Рендер главного меню: заголовок "SANDBOX", кнопки PLAY, SETTINGS, ABOUT
                break;
                
            case GameState::PLAYING:
                // Рендер игры: сетка частиц, панель выбора материалов (с иконками и цветами вкладок)
                break;
                
            case GameState::SETTINGS:
                // Рендер настроек: ползунки размера поля (gridWidth/gridHeight), кисти
                break;
                
            case GameState::ABOUT:
                // Рендер экрана "О программе"
                break;
        }
    }
};

// Точка входа строго снаружи класса
int main(int argc, char* argv[]) {
    SandboxApp app;
    return 0;
}
