// gui.cpp
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>

const int GRID_W = 120;
const int GRID_H = 80;

enum Element {
    EMPTY = 0,
    SAND = 1,
    WATER = 2,
    STONE = 3
};

static std::vector<int> grid(GRID_W * GRID_H, EMPTY);
static int currentElement = SAND;

void ClearGrid() {
    std::fill(grid.begin(), grid.end(), EMPTY);
}

void UpdateSandbox() {
    std::vector<int> newGrid = grid;

    for (int y = GRID_H - 1; y >= 0; --y) {
        for (int x = 0; x < GRID_W; ++x) {
            int idx = y * GRID_W + x;
            int type = grid[idx];

            if (type == SAND) {
                if (y + 1 < GRID_H) {
                    int belowIdx = (y + 1) * GRID_W + x;
                    if (grid[belowIdx] == EMPTY || grid[belowIdx] == WATER) {
                        newGrid[idx] = newGrid[belowIdx];
                        newGrid[belowIdx] = SAND;
                    } else {
                        bool leftOk = (x > 0 && grid[(y + 1) * GRID_W + (x - 1)] == EMPTY);
                        bool rightOk = (x < GRID_W - 1 && grid[(y + 1) * GRID_W + (x + 1)] == EMPTY);

                        if (leftOk && rightOk) {
                            int dir = (rand() % 2 == 0) ? -1 : 1;
                            newGrid[idx] = EMPTY;
                            newGrid[(y + 1) * GRID_W + (x + dir)] = SAND;
                        } else if (leftOk) {
                            newGrid[idx] = EMPTY;
                            newGrid[(y + 1) * GRID_W + (x - 1)] = SAND;
                        } else if (rightOk) {
                            newGrid[idx] = EMPTY;
                            newGrid[(y + 1) * GRID_W + (x + 1)] = SAND;
                        }
                    }
                }
            } else if (type == WATER) {
                if (y + 1 < GRID_H && grid[(y + 1) * GRID_W + x] == EMPTY) {
                    newGrid[idx] = EMPTY;
                    newGrid[(y + 1) * GRID_W + x] = WATER;
                } else {
                    int dir = (rand() % 2 == 0) ? -1 : 1;
                    if (x + dir >= 0 && x + dir < GRID_W && grid[y * GRID_W + (x + dir)] == EMPTY) {
                        newGrid[idx] = EMPTY;
                        newGrid[y * GRID_W + (x + dir)] = WATER;
                    } else if (x - dir >= 0 && x - dir < GRID_W && grid[y * GRID_W + (x - dir)] == EMPTY) {
                        newGrid[idx] = EMPTY;
                        newGrid[y * GRID_W + (x - dir)] = WATER;
                    }
                }
            }
        }
    }
    grid = newGrid;
}

bool InitGUI(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplGlfw_InitForOpenGL(window, true))
        return false;
    if (!ImGui_ImplOpenGL3_Init("#version 330"))
        return false;

    return true;
}

void RenderGUI() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    UpdateSandbox();

    ImGui::Begin("Sandbox Panel");
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Separator();
    
    ImGui::RadioButton("Песок (Sand)", &currentElement, SAND);
    ImGui::RadioButton("Вода (Water)", &currentElement, WATER);
    ImGui::RadioButton("Камень (Stone)", &currentElement, STONE);
    ImGui::RadioButton("Ластик (Eraser)", &currentElement, EMPTY);

    if (ImGui::Button("Очистить поле")) {
        ClearGrid();
    }
    ImGui::End();

    ImGui::Begin("Sandbox Area", nullptr, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
    
    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImVec2 canvasSize = ImGui::GetContentRegionAvail();
    
    if (canvasSize.x < 100) canvasSize.x = 600;
    if (canvasSize.y < 100) canvasSize.y = 500;

    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(20, 20, 20, 255));

    float cellW = canvasSize.x / GRID_W;
    float cellH = canvasSize.y / GRID_H;

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImVec2 mousePos = ImGui::GetMousePos();
        int mouseX = (int)((mousePos.x - canvasPos.x) / cellW);
        int mouseY = (int)((mousePos.y - canvasPos.y) / cellH);

        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                int tx = mouseX + dx;
                int ty = mouseY + dy;
                if (tx >= 0 && tx < GRID_W && ty >= 0 && ty < GRID_H) {
                    grid[ty * GRID_W + tx] = currentElement;
                }
            }
        }
    }

    for (int y = 0; y < GRID_H; ++y) {
        for (int x = 0; x < GRID_W; ++x) {
            int type = grid[y * GRID_W + x];
            if (type == EMPTY) continue;

            ImU32 color = IM_COL32(0, 0, 0, 255);
            if (type == SAND) color = IM_COL32(220, 190, 80, 255);
            else if (type == WATER) color = IM_COL32(50, 120, 240, 255);
            else if (type == STONE) color = IM_COL32(130, 130, 130, 255);

            ImVec2 pMin(canvasPos.x + x * cellW, canvasPos.y + y * cellH);
            ImVec2 pMax(pMin.x + cellW + 0.5f, pMin.y + cellH + 0.5f);
            drawList->AddRectFilled(pMin, pMax, color);
        }
    }

    ImGui::End();

    ImGui::Render();
    
    int display_w, display_h;
    GLFWwindow* window = glfwGetCurrentContext();
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

void CleanupGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}
