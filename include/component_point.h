#pragma once

#include <functional>

struct Point2D
{
    int x {};
    int y {};
};

struct Point2Df
{
    float x {};
    float y {};
};

// Same as Point2D, but x and y are column and row.
struct Cell2D
{
    int col {};
    int row {};

    bool operator==(const Cell2D& otherCell) const // Used for unordered_map<Cell2D>.
    {
        return col == otherCell.col && row == otherCell.row;
    }
};

struct Cell2DHash // Simple custom hash functor for unordered_map<Cell2D, Cell2DHash>. A functor is an object that acts like a function.
{
    /* 
    Hash of col
    XOR
    Hash of row shifted left 1.
    */
    size_t operator()(const Cell2D& key) const noexcept
    {
        size_t h1 = std::hash<int>{}(key.col);
        size_t h2 = std::hash<int>{}(key.row);
        return h1 ^ (h2 << 1);

        //return std::hash<int>()(key.col) ^ (std::hash<int>()(key.row) << 1);
    }
};