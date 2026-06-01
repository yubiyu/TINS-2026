#pragma once

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

#include <vector>

struct Audio
{
    static ALLEGRO_SAMPLE_INSTANCE *activeBgmInstance;
    static std::vector<ALLEGRO_SAMPLE_INSTANCE *> activeSfxInstances;

    static ALLEGRO_SAMPLE *reformatBgm;

    static float bgmGain;
    static float sfxGain;

    static void Initialize();
    static void Uninitialize();

    static void LoadResources();
    static void UnloadResources();

    static void SetBgmGain(float gain);
    static void SetSfxGain(float gain);

    static void AddSfx(ALLEGRO_SAMPLE *whichSfx);
    static void SetActiveBgm(ALLEGRO_SAMPLE *whichBgm, unsigned int position_secs);

    static void Update();
};
