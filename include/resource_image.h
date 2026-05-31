#pragma once

#include <allegro5/allegro.h>

#include <vector>

/*
An inventory of all external image resources, organized by the UI they appear in.
Functions for managing image resources.
*/
struct Image
{
/*
Settings
*/
    static ALLEGRO_BITMAP *settingsVolumeBarFullPng;
    static ALLEGRO_BITMAP *settingsVolumeBarEmptyPng;

/*
WorldView
*/
    static ALLEGRO_BITMAP *gridFramePng;
    static ALLEGRO_BITMAP *gridPng;

    static ALLEGRO_BITMAP *polarityButtonPng;
    static ALLEGRO_BITMAP *polarityButtonFramePng;

    static ALLEGRO_BITMAP *tachyonBarPng;
    static ALLEGRO_BITMAP *tachyonBarFramePng;
    
    static ALLEGRO_BITMAP *mimicAtlasPng;
    static std::vector<ALLEGRO_BITMAP*> mimicAtlas_mimicsA;
    static std::vector<ALLEGRO_BITMAP*> mimicAtlas_mimicsB;
    static std::vector<ALLEGRO_BITMAP*> mimicAtlas_phasingMimics;
    static std::vector<ALLEGRO_BITMAP*> mimicAtlas_unknownMimics;
    
    static ALLEGRO_BITMAP *pupilAtlasPng;
    static std::vector<ALLEGRO_BITMAP*> pupilAtlas;

    static ALLEGRO_BITMAP *captureAtlasPng;
    static std::vector<ALLEGRO_BITMAP*> captureAtlas;
    static std::vector<ALLEGRO_BITMAP*> captureAtlas_mask;

    static ALLEGRO_BITMAP *radiationSmallPng;
    static ALLEGRO_BITMAP *radiationLargePng;
    static std::vector<ALLEGRO_BITMAP*> radiationAtlas;

    static void Initialize();
    static void Uninitialize();

    static void LoadResources();
    static void UnloadResources();
};
