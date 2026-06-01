#include "core_filesystem.h"

#include <iostream>

void Filesystem::Initialize(char **argv)
{
    PHYSFS_init(argv[0]);

    if (PHYSFS_mount("data/image.zip", "/", 1) == 0)
        std::cout << "Error - Filesystem: image.zip not found." << std::endl;
    else
        std::cout << "Filesystem: image.zip successfully mounted." << std::endl;

    if (PHYSFS_mount("data/font.zip", "/", 1) == 0)
        std::cout << "Error - Filesystem: font.zip not found." << std::endl;
    else
        std::cout << "Filesystem: font.zip successfully mounted." << std::endl;

    if (PHYSFS_mount("data/audio.zip", "/", 1) == 0)
        std::cout << "Error - Filesystem: audio.zip not found." << std::endl;
    else
        std::cout << "Filesystem: audio.zip successfully mounted." << std::endl;
}
void Filesystem::Uninitialize()
{
    PHYSFS_deinit();
}

void Filesystem::SetStandardFileInterface()
{
    al_set_standard_file_interface();
}
void Filesystem::SetPhysfsFileInterface()
{
    al_set_physfs_file_interface();
}