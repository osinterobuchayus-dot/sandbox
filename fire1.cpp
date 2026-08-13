#include <cstdlib>

void update_fire(int x, int y, int width, int height, int** grid) {
    const int EMPTY = 0;
    const int FIRE = 3;
    const int WATER = 4;
    const int SUT = 6;
    const int SMOKE = 8; // Дым

    // Реакция с водой (тушение -> нагар + пар)
    int dx[] = {0, 0, -1, 1};
    int dy[] = {-1, 1, 0, 0};
    for (int i = 0; i < 4; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        if (nx >= 0 && nx < width && ny >= 0 && ny < height) {
            if (grid[nx][ny] == WATER) {
                grid[x][y] = SUT;
                grid[nx][ny] = 7; // Пар (ID 7)
                return;
            }
        }
    }

    // Генерация дыма сверху от огня (с небольшим шансом, чтобы дым шел густо, но не сплошной стеной)
    if (y - 1 >= 0 && grid[x][y - 1] == EMPTY && rand() % 8 == 0) {
        grid[x][y - 1] = SMOKE;
    }

    // Движение огня вверх
    if (y - 1 >= 0) {
        if (grid[x][y - 1] == EMPTY) {
            grid[x][y - 1] = FIRE;
            grid[x][y] = EMPTY;
            return;
        }
        int dir = (rand() % 2 == 0) ? -1 : 1;
        if (x + dir >= 0 && x + dir < width && grid[x + dir][y - 1] == EMPTY) {
            grid[x + dir][y - 1] = FIRE;
            grid[x][y] = EMPTY;
            return;
        }
        if (x - dir >= 0 && x - dir < width && grid[x - dir][y - 1] == EMPTY) {
            grid[x - dir][y - 1] = FIRE;
            grid[x][y] = EMPTY;
            return;
        }
    }
}
