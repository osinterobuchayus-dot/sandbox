#include <cstdlib> // Для rand()

// Пытается упасть вниз. Если некуда — растекается в стороны.
// Течет быстрее, чем падает.
void update_water(int x, int y, int width, int height, int** grid) {
    if (y + 1 >= height) return;

    const int EMPTY = 0;
    const int WATER = 4;

    // 1. Пробуем упасть строго вниз (высший приоритет)
    if (grid[x][y + 1] == EMPTY) {
        grid[x][y + 1] = WATER;
        grid[x][y] = EMPTY;
        return;
    }

    // 2. Если вниз нельзя, пробуем течь по диагонали вниз (второй приоритет)
    int dir = (rand() % 2 == 0) ? -1 : 1;

    int nx1 = x + dir;
    if (nx1 >= 0 && nx1 < width) {
        if (grid[nx1][y + 1] == EMPTY) {
            grid[nx1][y + 1] = WATER;
            grid[x][y] = EMPTY;
            return;
        }
    }

    int nx2 = x - dir;
    if (nx2 >= 0 && nx2 < width) {
        if (grid[nx2][y + 1] == EMPTY) {
            grid[nx2][y + 1] = WATER;
            grid[x][y] = EMPTY;
            return;
        }
    }

    // 3. Если вниз и по диагонали вниз нельзя, пробуем течь строго в бок (низкий приоритет)
    if (nx1 >= 0 && nx1 < width && grid[nx1][y] == EMPTY) {
        grid[nx1][y] = WATER;
        grid[x][y] = EMPTY;
        return;
    }

    if (nx2 >= 0 && nx2 < width && grid[nx2][y] == EMPTY) {
        grid[nx2][y] = WATER;
        grid[x][y] = EMPTY;
        return;
    }
    // Если вода заперта, она не двигается
}
