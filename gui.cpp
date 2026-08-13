#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <cstdio>

const int GRID_W = 160;
const int GRID_H = 100;

enum Element {
    EMPTY = 0,
    SAND,
    WATER,
    STONE,
    METAL,
    WOOD,
    OIL,
    ACID,
    ICE,
    FIRE
};

static std::vector<int> grid(GRID_W * GRID_H, EMPTY);
static int currentElement = SAND;
static int brushSize = 2;

void ClearGrid() {
    std::fill(grid.begin(), grid.end(), EMPTY);
}

void UpdateSandbox() {
    std::vector<int> newGrid = grid;

    for (int y = GRID_H - 1; y >= 0; --y) {
        for (int x_iter = 0; x_iter < GRID_W; ++x_iter) {
            int x = (y % 2 == 0) ? x_iter : (GRID_W - 1 - x_iter);
            int idx = y * GRID_W + x;
            int type = grid[idx];

            if (type == EMPTY || type == STONE || type == METAL) continue;

            if (type == SAND) {
                if (y + 1 < GRID_H) {
                    int below = grid[(y + 1) * GRID_W + x];
                    if (below == EMPTY) {
                        newGrid[idx] = EMPTY;
                        newGrid[(y + 1) * GRID_W + x] = SAND;
                    } else if (below == WATER || below == OIL) {
                        newGrid[idx] = below;
                        newGrid[(y + 1) * GRID_W + x] = SAND;
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
            } else if (type == WATER || type == ACID) {
                if (y + 1 < GRID_H && grid[(y + 1) * GRID_W + x] == EMPTY) {
                    newGrid[idx] = EMPTY;
                    newGrid[(y + 1) * GRID_W + x] = type;
                } else {
                    bool leftOk = (x > 0 && grid[(y + 1) * GRID_W + (x - 1)] == EMPTY);
                    bool rightOk = (x < GRID_W - 1 && grid[(y + 1) * GRID_W + (x + 1)] == EMPTY);
                    if (leftOk && rightOk) {
                        int dir = (rand() % 2 == 0) ? -1 : 1;
                        newGrid[idx] = EMPTY;
                        newGrid[(y + 1) * GRID_W + (x + dir)] = type;
                    } else if (leftOk) {
                        newGrid[idx] = EMPTY;
                        newGrid[(y + 1) * GRID_W + (x - 1)] = type;
                    } else if (rightOk) {
                        newGrid[idx] = EMPTY;
                        newGrid[(y + 1) * GRID_W + (x + 1)] = type;
                    } else {
                        int dir = (rand() % 2 == 0) ? -1 : 1;
                        if (x + dir >= 0 && x + dir < GRID_W && grid[y * GRID_W + (x + dir)] == EMPTY) {
                            newGrid[idx] = EMPTY;
                            newGrid[y * GRID_W + (x + dir)] = type;
                        } else if (x - dir >= 0 && x - dir < GRID_W && grid[y * GRID_W + (x - dir)] == EMPTY) {
                            newGrid[idx] = EMPTY;
                            newGrid[y * GRID_W + (x - dir)] = type;
                        }
                    }
                }
            } else if (type == OIL) {
                if (y + 1 < GRID_H) {
                    int below = grid[(y + 1) * GRID_W + x];
                    if (below == EMPTY || below == WATER) {
                        newGrid[idx] = below;
                        newGrid[(y + 1) * GRID_W + x] = OIL;
                    } else {
                        int dir = (rand() % 2 == 0) ? -1 : 1;
                        if (x + dir >= 0 && x + dir < GRID_W && grid[y * GRID_W + (x + dir)] == EMPTY) {
                            newGrid[idx] = EMPTY;
                            newGrid[y * GRID_W + (x + dir)] = OIL;
                        }
                    }
                }
            } else if (type == FIRE) {
                if (rand() % 3 == 0) {
                    newGrid[idx] = EMPTY;
                } else {
                    int nx = x + (rand() % 3 - 1);
                    int ny = y - 1;
                    if (nx >= 0 && nx < GRID_W && ny >= 0) {
                        int target = grid[ny * GRID_W + nx];
                        if (target == WOOD || target == OIL) {
                            newGrid[ny * GRID_W + nx] = FIRE;
                        } else if (target == EMPTY) {
                            newGrid[idx] = EMPTY;
                            newGrid[ny * GRID_W + nx] = FIRE;
                        }
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

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 0.0f;
    style.FrameRounding = 3.0f;

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

    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);

    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoMove |
                                    ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoBringToFrontOnFocus |
                                    ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_MenuBar;

    ImGui::Begin("PixelSandboxMain", nullptr, window_flags);

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Clear Grid")) {
                ClearGrid();
            }
            ImGui::EndMenu();
        }
        ImGui::Text(" | FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::EndMenuBar();
    }

    ImVec2 region = ImGui::GetContentRegionAvail();
    float toolbarHeight = 110.0f;
    ImVec2 canvasSize(region.x, region.y - toolbarHeight);

    ImVec2 canvasPos = ImGui::GetCursorScreenPos();
    ImDrawList* drawList = ImGui::GetWindowDrawList();
    drawList->AddRectFilled(canvasPos, ImVec2(canvasPos.x + canvasSize.x, canvasPos.y + canvasSize.y), IM_COL32(18, 18, 18, 255));

    float cellW = canvasSize.x / GRID_W;
    float cellH = canvasSize.y / GRID_H;

    if (ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Left)) {
        ImVec2 mousePos = ImGui::GetMousePos();
        if (mousePos.x >= canvasPos.x && mousePos.x < canvasPos.x + canvasSize.x &&
            mousePos.y >= canvasPos.y && mousePos.y < canvasPos.y + canvasSize.y) {
            int mouseX = (int)((mousePos.x - canvasPos.x) / cellW);
            int mouseY = (int)((mousePos.y - canvasPos.y) / cellH);

            for (int dy = -brushSize; dy <= brushSize; ++dy) {
                for (int dx = -brushSize; dx <= brushSize; ++dx) {
                    int tx = mouseX + dx;
                    int ty = mouseY + dy;
                    if (tx >= 0 && tx < GRID_W && ty >= 0 && ty < GRID_H) {
                        if (dx*dx + dy*dy <= brushSize * brushSize + 1) {
                            grid[ty * GRID_W + tx] = currentElement;
                        }
                    }
                }
            }
        }
    }

    for (int y = 0; y < GRID_H; ++y) {
        for (int x = 0; x < GRID_W; ++x) {
            int type = grid[y * GRID_W + x];
            if (type == EMPTY) continue;

            ImU32 color = IM_COL32(0, 0, 0, 255);
            switch (type) {
                case SAND:  color = IM_COL32(225, 195, 80, 255); break;
                case WATER: color = IM_COL32(50, 130, 250, 255); break;
                case STONE: color = IM_COL32(120, 120, 120, 255); break;
                case METAL: color = IM_COL32(170, 175, 190, 255); break;
                case WOOD:  color = IM_COL32(135, 85, 40, 255); break;
                case OIL:   color = IM_COL32(85, 75, 45, 255); break;
                case ACID:  color = IM_COL32(40, 240, 40, 255); break;
                case ICE:   color = IM_COL32(175, 225, 245, 255); break;
                case FIRE:  color = IM_COL32(250, 95, 15, 255); break;
            }

            ImVec2 pMin(canvasPos.x + x * cellW, canvasPos.y + y * cellH);
            ImVec2 pMax(pMin.x + cellW + 0.5f, pMin.y + cellH + 0.5f);
            drawList->AddRectFilled(pMin, pMax, color);
        }
    }

    ImGui::Dummy(canvasSize);
    ImGui::Spacing();

    ImGui::BeginChild("BottomToolbar", ImVec2(0, 0), true);
    
    ImGui::SetNextItemWidth(180.0f);
    ImGui::SliderInt("Brush Size", &brushSize, 1, 6);
    ImGui::SameLine();
    ImGui::Spacing();
    ImGui::SameLine();
    if (ImGui::Button("Clear All Fields", ImVec2(140, 28))) {
        ClearGrid();
    }

    ImGui::Spacing();

    struct MaterialInfo {
        const char* name;
        int id;
        ImVec4 color;
    };

    MaterialInfo materials[] = {
        { "Eraser", EMPTY, ImVec4(0.25f, 0.25f, 0.25f, 1.0f) },
        { "Sand", SAND, ImVec4(0.88f, 0.76f, 0.32f, 1.0f) },
        { "Water", WATER, ImVec4(0.2f, 0.5f, 0.98f, 1.0f) },
        { "Stone", STONE, ImVec4(0.5f, 0.5f, 0.5f, 1.0f) },
        { "Metal", METAL, ImVec4(0.72f, 0.75f, 0.8f, 1.0f) },
        { "Wood", WOOD, ImVec4(0.55f, 0.35f, 0.17f, 1.0f) },
        { "Oil", OIL, ImVec4(0.35f, 0.3f, 0.2f, 1.0f) },
        { "Acid", ACID, ImVec4(0.2f, 0.92f, 0.2f, 1.0f) },
        { "Ice", ICE, ImVec4(0.7f, 0.9f, 0.95f, 1.0f) },
        { "Fire", FIRE, ImVec4(0.96f, 0.4f, 0.1f, 1.0f) }
    };

    int i = 0;
    for (auto& mat : materials) {
        bool isSelected = (currentElement == mat.id);
        ImGui::PushStyleColor(ImGuiCol_Button, mat.color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(mat.color.x * 1.15f, mat.color.y * 1.15f, mat.color.z * 1.15f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(mat.color.x * 0.85f, mat.color.y * 0.85f, mat.color.z * 0.85f, 1.0f));

        char label[64];
        snprintf(label, sizeof(label), "%s%s", mat.name, isSelected ? " [X]" : "");

        if (ImGui::Button(label, ImVec2(90, 28))) {
            currentElement = mat.id;
        }
        ImGui::PopStyleColor(3);

        if ((i + 1) % 10 != 0) {
            ImGui::SameLine();
        }
        i++;
    }

    ImGui::EndChild();
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
