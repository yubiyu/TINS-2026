#include "resource_image.h"

#include "data_tile.h"
#include "data_mimic.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <iostream>

ALLEGRO_BITMAP *Image::settingsVolumeBarFullPng;
ALLEGRO_BITMAP *Image::settingsVolumeBarEmptyPng;

ALLEGRO_BITMAP *Image::gridFramePng;
ALLEGRO_BITMAP *Image::gridPng;

ALLEGRO_BITMAP *Image::mimicAtlasPng;
std::vector<ALLEGRO_BITMAP*> Image::mimicAtlas_mimics;

ALLEGRO_BITMAP *Image::captureAtlasPng;

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

    gridFramePng = al_load_bitmap("gridFrame.png");
    gridPng = al_load_bitmap("grid.png");

    mimicAtlasPng = al_load_bitmap("mimicAtlas.png");
    for(size_t i = 0; i < MimicData::NUM_CASTES; i++)
        mimicAtlas_mimics.push_back( al_create_sub_bitmap(mimicAtlasPng, MimicData::SPRITE_WIDTH* i, 0, MimicData::SPRITE_WIDTH, MimicData::SPRITE_HEIGHT));

    captureAtlasPng = al_load_bitmap("captureAtlas.png");

}

void Image::UnloadResources()
{
    std::cout << "Image: Unloading resources." << std::endl;

    al_destroy_bitmap(settingsVolumeBarFullPng);
    al_destroy_bitmap(settingsVolumeBarEmptyPng);

    al_destroy_bitmap(gridFramePng);
    al_destroy_bitmap(gridPng);
    al_destroy_bitmap(mimicAtlasPng);
    al_destroy_bitmap(captureAtlasPng);
}
