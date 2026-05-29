#include "resource_image.h"

#include "data_tile.h"
#include "data_sprite.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <iostream>

ALLEGRO_BITMAP *Image::settingsVolumeBarFullPng;
ALLEGRO_BITMAP *Image::settingsVolumeBarEmptyPng;

void Image::Initialize()
{
    al_init_image_addon();
    al_init_primitives_addon();

    LoadResources();
}

void Image::Uninitialize()
{
    UnloadResources();

    al_shutdown_primitives_addon();
    al_shutdown_image_addon();
}

void Image::LoadResources()
{
    std::cout << "Image: Loading resources." << std::endl;

    settingsVolumeBarFullPng = al_load_bitmap("settingsVolumeBarFull.png");
    settingsVolumeBarEmptyPng = al_load_bitmap("settingsVolumeBarEmpty.png");
}

void Image::UnloadResources()
{
    std::cout << "Image: Unloading resources." << std::endl;

    al_destroy_bitmap(settingsVolumeBarFullPng);
    al_destroy_bitmap(settingsVolumeBarEmptyPng);
}
