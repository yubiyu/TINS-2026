#include "resource_image.h"

#include "data_tile.h"
#include "data_mimic.h"
#include "data_field.h"

#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>

#include <iostream>

ALLEGRO_BITMAP *Image::settingsVolumeBarFullPng;
ALLEGRO_BITMAP *Image::settingsVolumeBarEmptyPng;

ALLEGRO_BITMAP *Image::gridFramePng;
ALLEGRO_BITMAP *Image::gridPng;

ALLEGRO_BITMAP *Image::revertButtonPng;
ALLEGRO_BITMAP *Image::revertButtonFramePng;

ALLEGRO_BITMAP *Image::tachyonBarPng;
ALLEGRO_BITMAP *Image::tachyonBarFramePng;

ALLEGRO_BITMAP *Image::dialogRectPng;

ALLEGRO_BITMAP *Image::mimicAtlasPng;
std::vector<ALLEGRO_BITMAP *> Image::mimicAtlas_mimicsA;
std::vector<ALLEGRO_BITMAP *> Image::mimicAtlas_mimicsB;
std::vector<ALLEGRO_BITMAP *> Image::mimicAtlas_phasingMimics;
std::vector<ALLEGRO_BITMAP *> Image::mimicAtlas_unknownMimics;

ALLEGRO_BITMAP *Image::pupilAtlasPng;
std::vector<ALLEGRO_BITMAP *> Image::pupilAtlas;

ALLEGRO_BITMAP *Image::bubblePng;

ALLEGRO_BITMAP *Image::captureAtlasPng;
std::vector<ALLEGRO_BITMAP *> Image::captureAtlas;
std::vector<ALLEGRO_BITMAP *> Image::captureAtlas_mask;

ALLEGRO_BITMAP *Image::radiationSmallPng;
ALLEGRO_BITMAP *Image::radiationLargePng;
ALLEGRO_BITMAP *Image::shrapenelPng;
std::vector<ALLEGRO_BITMAP*> Image::radiationAtlas;

ALLEGRO_BITMAP *Image::stunLightningAtlasPng;
std::vector<ALLEGRO_BITMAP*> Image::stunLightningAtlas;

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

    revertButtonPng = al_load_bitmap("revertButton.png");
    revertButtonFramePng = al_load_bitmap("revertButtonFrame.png");

    tachyonBarPng = al_load_bitmap("tachyonBar.png");
    tachyonBarFramePng = al_load_bitmap("tachyonBarFrame.png");

    dialogRectPng = al_load_bitmap("dialogRect.png");

    mimicAtlasPng = al_load_bitmap("mimicAtlas.png");
    for (size_t i = 0; i < MimicData::NUM_CLADES; i++)
    {
        mimicAtlas_mimicsA.push_back(al_create_sub_bitmap(mimicAtlasPng, MimicData::SPRITE_WIDTH * i, MimicData::SPRITE_HEIGHT * 0, MimicData::SPRITE_WIDTH, MimicData::SPRITE_HEIGHT));
        mimicAtlas_mimicsB.push_back(al_create_sub_bitmap(mimicAtlasPng, MimicData::SPRITE_WIDTH * i, MimicData::SPRITE_HEIGHT * 1, MimicData::SPRITE_WIDTH, MimicData::SPRITE_HEIGHT));
        mimicAtlas_phasingMimics.push_back(al_create_sub_bitmap(mimicAtlasPng, MimicData::SPRITE_WIDTH * i, MimicData::SPRITE_HEIGHT * 2, MimicData::SPRITE_WIDTH, MimicData::SPRITE_HEIGHT));
        mimicAtlas_unknownMimics.push_back(al_create_sub_bitmap(mimicAtlasPng, MimicData::SPRITE_WIDTH * i, MimicData::SPRITE_HEIGHT * 3, MimicData::SPRITE_WIDTH, MimicData::SPRITE_HEIGHT));
    }

    pupilAtlasPng = al_load_bitmap("pupilAtlas.png");
    for (size_t y = 0; y < MimicData::NUM_PUPIL_SHAPES; y++)
        for(size_t x = 0; x < MimicData::NUM_PUPIL_VARIANTS; x++)
            pupilAtlas.push_back( al_create_sub_bitmap(pupilAtlasPng, x*MimicData::SPRITE_WIDTH, y*MimicData::SPRITE_HEIGHT, MimicData::SPRITE_WIDTH, MimicData::SPRITE_HEIGHT));

    bubblePng = al_load_bitmap("bubble.png");

    captureAtlasPng = al_load_bitmap("captureAtlas.png");
    for (size_t i = 0; i < FieldData::CAPTURE_ANIMATION_NUM_FRAMES; i++)
    {
        captureAtlas.push_back(al_create_sub_bitmap(captureAtlasPng, FieldData::CELL_WIDTH * i, FieldData::CELL_HEIGHT * 0, FieldData::CELL_WIDTH, FieldData::CELL_HEIGHT));
        captureAtlas_mask.push_back(al_create_sub_bitmap(captureAtlasPng, FieldData::CELL_WIDTH * i, FieldData::CELL_HEIGHT * 1, FieldData::CELL_WIDTH, FieldData::CELL_HEIGHT));
    }

    radiationSmallPng = al_load_bitmap("radiationSmall.png");
    radiationLargePng = al_load_bitmap("radiationLarge.png");
    shrapenelPng = al_load_bitmap("shrapenel.png");
    for(size_t i = 0; i < 2; i++)
        radiationAtlas.push_back(al_create_sub_bitmap(radiationSmallPng, i*3, 0, 3, 3));
    for(size_t i = 0; i < 2; i++)
        radiationAtlas.push_back(al_create_sub_bitmap(radiationLargePng, i*4, 0, 4, 4));
    for(size_t i = 0; i < 2; i++)
        radiationAtlas.push_back(al_create_sub_bitmap(shrapenelPng, i*20, 0, 20, 20 ));

    stunLightningAtlasPng = al_load_bitmap("stunLightningAtlas.png");
    for(size_t i = 0; i < MimicData::NUM_STUN_LIGHTNING_VARIANTS; i++)
        stunLightningAtlas.push_back( al_create_sub_bitmap(stunLightningAtlasPng, i*MimicData::STUN_LIGHTNING_WIDTH, 0, MimicData::STUN_LIGHTNING_WIDTH, MimicData::STUN_LIGHTNING_HEIGHT));
}

void Image::UnloadResources()
{
    std::cout << "Image: Unloading resources." << std::endl;

    al_destroy_bitmap(settingsVolumeBarFullPng);
    al_destroy_bitmap(settingsVolumeBarEmptyPng);

    al_destroy_bitmap(gridFramePng);
    al_destroy_bitmap(gridPng);

    al_destroy_bitmap(revertButtonPng);
    al_destroy_bitmap(revertButtonFramePng);

    al_destroy_bitmap(tachyonBarPng);
    al_destroy_bitmap(tachyonBarFramePng);

    al_destroy_bitmap(dialogRectPng);

    for (ALLEGRO_BITMAP *b : mimicAtlas_mimicsA)
        al_destroy_bitmap(b);
    for (ALLEGRO_BITMAP *b : mimicAtlas_mimicsB)
        al_destroy_bitmap(b);
    for (ALLEGRO_BITMAP *b : mimicAtlas_phasingMimics)
        al_destroy_bitmap(b);
    for (ALLEGRO_BITMAP *b : mimicAtlas_unknownMimics)
        al_destroy_bitmap(b);
    al_destroy_bitmap(mimicAtlasPng);

    for (ALLEGRO_BITMAP *b : pupilAtlas)
        al_destroy_bitmap(b);
    al_destroy_bitmap(pupilAtlasPng);

    al_destroy_bitmap(bubblePng);

    for (ALLEGRO_BITMAP *b : captureAtlas)
        al_destroy_bitmap(b);
    for (ALLEGRO_BITMAP *b : captureAtlas_mask)
        al_destroy_bitmap(b);
    al_destroy_bitmap(captureAtlasPng);

    for (ALLEGRO_BITMAP *b : radiationAtlas)
        al_destroy_bitmap(b);
    al_destroy_bitmap(radiationSmallPng);
    al_destroy_bitmap(radiationLargePng);
    al_destroy_bitmap(shrapenelPng);

    for (ALLEGRO_BITMAP *b : stunLightningAtlas)
        al_destroy_bitmap(b);
    al_destroy_bitmap(stunLightningAtlasPng);


}
