// Падает строго вниз.
// В отличие от песка, не скатывается в стороны.
// Падает сквозь воду и огонь.
void update_stone(int x, int y, int width, int height, int** grid) {
    if (y + 1 >= height) return;

    const int EMPTY = 0;
    const int STONE = 1;
    const int FIRE = 3;
    const int WATER = 4;

    // Камень падает только если внизу пусто, вода или огонь
    int below = grid[x][y + 1];
    if (below == EMPTY || below == WATER || below == FIRE) {
        // Меняем местами камень с тем, что под ним
        grid[x][y + 1] = STONE;
        grid[x][y] = below;
    }
    // Если внизу другой камень или песок — камень останавливается
}
