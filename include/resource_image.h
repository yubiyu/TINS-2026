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
    
    static ALLEGRO_BITMAP *mimicAtlasPng;
    static std::vector<ALLEGRO_BITMAP*> mimicAtlas_mimics;

    static ALLEGRO_BITMAP *captureAtlasPng;

    static void Initialize();
    static void Uninitialize();

    static void LoadResources();
    static void UnloadResources();
};
