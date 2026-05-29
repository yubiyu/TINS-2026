#pragma once

#include "data_direction.h"

/*
    Purposes:

1) Represents the world as a graph of Chunk connections.
Distances on the graph do not translate to x/y/z distances on a grid.
However, the Chunks (see world_chunk.h) that form the graph's nodes do in fact have their own world positions.

2) Track and coordinate Creatures.

3) Track and coordinate Items.

There is only supposed to be a single static WorldModel instance at a time.
*/
class WorldModel
{

public:
    static WorldModel world; // Shared world instance.

    void Initialize();
    void Uninitialize();

    void Update();
};