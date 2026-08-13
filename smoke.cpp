#include <cstdlib>

void update_smoke(int x, int y, int width, int height, int** grid) {
    const int EMPTY = 0;
    const int SMOKE = 8;

    // Дым постепенно рассеивается на верху экрана или со временем
    if (y <= 2 || rand() % 150 == 0) {
        grid[x][y] = EMPTY;
        return;
    }

    if (y - 1 >= 0) {
        // 1. Приоритет: строго вверх
        if (grid[x][y - 1] == EMPTY) {
            grid[x][y - 1] = SMOKE;
            grid[x][y] = EMPTY;
            return;
        }

        // 2. Диагональ вверх
        int dir = (rand() % 2 == 0) ? -1 : 1;
        if (x + dir >= 0 && x + dir < width && grid[x + dir][y - 1] == EMPTY) {
            grid[x + dir][y - 1] = SMOKE;
            grid[x][y] = EMPTY;
            return;
        }
        if (x - dir >= 0 && x - dir < width && grid[x - dir][y - 1] == EMPTY) {
            grid[x - dir][y - 1] = SMOKE;
            grid[x][y] = EMPTY;
            return;
        }

        // 3. Активное движение в бока (чтобы дым стелился по потолку и находил окно)
        if (x + dir >= 0 && x + dir < width && grid[x + dir][y] == EMPTY) {
            grid[x + dir][y] = SMOKE;
            grid[x][y] = EMPTY;
            return;
        }
        if (x - dir >= 0 && x - dir < width && grid[x - dir][y] == EMPTY) {
            grid[x - dir][y] = SMOKE;
            grid[x][y] = EMPTY;
            return;
        }
    }
}
