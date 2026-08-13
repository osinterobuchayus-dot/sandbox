#include "MaterialPhysics.h"
#include <cstdlib>
#include <vector>

class SandPhysics : public MaterialPhysics {
private:
    // Проверка соседних клеток на наличие огня (ID = 5)
    bool isTouchingFire(int x, int y, const std::vector<std::vector<int>>& grid, int width, int height) {
        int dx[] = {-1, 1, 0, 0, -1, 1, -1, 1};
        int dy[] = {0, 0, -1, 1, -1, -1, 1, 1};
        
        for (int i = 0; i < 8; ++i) {
            int nx = x + dx[i];
            int ny = y + dy[i];
            if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
                if (grid[ny][nx] == 5) { // 5 — ID огня
                    return true;
                }
            }
        }
        return false;
    }

public:
    void update(int x, int y, std::vector<std::vector<int>>& grid, int width, int height) override {
        // 1. Проверяем нагрев: если рядом огонь, превращаемся в стекло (ID = 4)
        if (isTouchingFire(x, y, grid, width, height)) {
            grid[y][x] = 4; // ID стекла
            return;
        }

        // 2. Проверка выхода за границы сетки по вертикали
        if (y + 1 >= height) return;

        // 3. Физика падения прямо вниз
        if (grid[y + 1][x] == 0) {
            grid[y + 1][x] = grid[y][x];
            grid[y][x] = 0;
        } 
        // 4. Физика скольжения по диагонали, если прямо занять
        else {
            int dir = (std::rand() % 2 == 0) ? 1 : -1;
            
            if (x + dir >= 0 && x + dir < width && grid[y + 1][x + dir] == 0) {
                grid[y + 1][x + dir] = grid[y][x];
                grid[y][x] = 0;
            } 
            else if (x - dir >= 0 && x - dir < width && grid[y + 1][x - dir] == 0) {
                grid[y + 1][x - dir] = grid[y][x];
                grid[y][x] = 0;
            }
        }
    }
};
