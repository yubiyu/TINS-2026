#pragma once

#include <allegro5/allegro_physfs.h>
#include <physfs.h>

/*
PHYSFS, a filesystem library, is used for the sole purpose of accessing the archive "data.zip"
which contains the game's external image, audio, and font resources.

Related is "configuration.h"
*/

struct Filesystem
{
    static void Initialize(char **argv);
    static void Uninitialize();
    static void SetStandardFileInterface();
    static void SetPhysfsFileInterface();
};
