#include <cstdlib>

void update_dirt(int x, int y, int width, int height, int** grid) {
    if (y + 1 >= height) return;

    const int EMPTY = 0;
    const int WATER = 4;
    const int DIRT = 5; // ID земли

    // 1. Падение строго вниз (пусто или вода)
    int below = grid[x][y + 1];
    if (below == EMPTY || below == WATER) {
        grid[x][y + 1] = DIRT;
        grid[x][y] = below;
        return;
    }

    // 2. Скольжение по диагонали вниз (чтобы ссыпалась в кучи)
    int dir = (rand() % 2 == 0) ? -1 : 1;

    // Первая попытка диагонали
    int nx1 = x + dir;
    if (nx1 >= 0 && nx1 < width) {
        int diag1 = grid[nx1][y + 1];
        if (diag1 == EMPTY || diag1 == WATER) {
            grid[nx1][y + 1] = DIRT;
            grid[x][y] = diag1;
            return;
        }
    }

    // Вторая попытка диагонали
    int nx2 = x - dir;
    if (nx2 >= 0 && nx2 < width) {
        int diag2 = grid[nx2][y + 1];
        if (diag2 == EMPTY || diag2 == WATER) {
            grid[nx2][y + 1] = DIRT;
            grid[x][y] = diag2;
            return;
        }
    }
}
