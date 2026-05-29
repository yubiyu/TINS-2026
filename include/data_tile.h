#pragma once

struct Tile
{
    static constexpr float WIDTH = 16;
    static constexpr float HEIGHT = 16;

    static constexpr float HALF_WIDTH = WIDTH / 2;
    static constexpr float HALF_HEIGHT = HEIGHT / 2;

    static constexpr float DOUBLE_WIDTH = WIDTH * 2;
    static constexpr float DOUBLE_HEIGHT = HEIGHT * 2;
};
