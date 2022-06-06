#pragma once

#include "vector"

class Cell {
public:
    sf::RectangleShape rect;
};

typedef typename std::vector<std::vector<Cell>> Cells;