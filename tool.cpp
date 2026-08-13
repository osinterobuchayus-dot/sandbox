#include <cmath>
#include <algorithm>

// 1. Обычная кисть (круглый штамп разного размера)
void tool_brush(int** grid, int width, int height, int cx, int cy, int radius, int material_id) {
    for (int y = cy - radius; y <= cy + radius; ++y) {
        for (int x = cx - radius; x <= cx + radius; ++x) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                if ((x - cx) * (x - cx) + (y - cy) * (y - cy) <= radius * radius) {
                    grid[x][y] = material_id;
                }
            }
        }
    }
}

// 2. Инструмент "Линия" (удобно для рисования наклонных перекрытий, труб) — Алгоритм Брезенхэма
void tool_line(int** grid, int width, int height, int x0, int y0, int x1, int y1, int material_id) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        if (x0 >= 0 && x0 < width && y0 >= 0 && y0 < height) {
            grid[x0][y0] = material_id;
        }
        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

// 3. Инструмент "Прямоугольник" (идеально для постройки комнат, стен и коробок с окнами)
// filled = true (сплошной прямоугольник/блок), filled = false (пустые стены комнаты)
void tool_rect(int** grid, int width, int height, int x0, int y0, int x1, int y1, int material_id, bool filled) {
    int min_x = std::min(x0, x1);
    int max_x = std::max(x0, x1);
    int min_y = std::min(y0, y1);
    int max_y = std::max(y0, y1);

    for (int y = min_y; y <= max_y; ++y) {
        for (int x = min_x; x <= max_x; ++x) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                if (filled || x == min_x || x == max_x || y == min_y || y == max_y) {
                    grid[x][y] = material_id;
                }
            }
        }
    }
}

// 4. Ластик (стирает всё в радиусе, превращая в пустоту ID 0)
void tool_eraser(int** grid, int width, int height, int cx, int cy, int radius) {
    tool_brush(grid, width, height, cx, cy, radius, 0); // 0 — пустота
}
