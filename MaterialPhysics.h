#pragma once

#include <vector>

// Общий базовый интерфейс для физики всех материалов
class MaterialPhysics {
public:
    virtual void update(int x, int y, std::vector<std::vector<int>>& grid, int width, int height) = 0;
    virtual ~MaterialPhysics() = default;
};
