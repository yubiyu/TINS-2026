#include "resource_audio.h"

#include <iostream>

ALLEGRO_SAMPLE_INSTANCE *Audio::activeBgmInstance;
std::vector<ALLEGRO_SAMPLE_INSTANCE *> Audio::activeSfxInstances;

//ALLEGRO_SAMPLE *Audio::treeBgm;

float Audio::bgmGain;
float Audio::sfxGain;

void Audio::Initialize()
{
    al_install_audio();
    al_init_acodec_addon();

    LoadResources();

    SetBgmGain(0.5);
    SetSfxGain(0.5);

    activeBgmInstance = nullptr;
}

void Audio::Uninitialize()
{
    for (std::vector<ALLEGRO_SAMPLE_INSTANCE *>::iterator it = activeSfxInstances.begin(); it != activeSfxInstances.end(); ++it)
    {
        al_destroy_sample_instance(*it);
    }
    activeSfxInstances.clear();

    al_destroy_sample_instance(activeBgmInstance);

    UnloadResources();
    al_uninstall_audio();
}

void Audio::LoadResources()
{
    std::cout << "Audio: Loading resources." << std::endl;
    al_reserve_samples(16);
    //treeBgm = al_load_sample("tree.ogg");
}

void Audio::UnloadResources()
{
    std::cout << "Audio: Unloading resources." << std::endl;
    //al_destroy_sample(treeBgm);
}

void Audio::SetBgmGain(float gain)
{
    bgmGain = gain;
    if (activeBgmInstance != nullptr)
    {
        al_set_sample_instance_gain(activeBgmInstance, bgmGain);
    }
}

void Audio::SetSfxGain(float gain)
{
    sfxGain = gain;
}

void Audio::AddSfx(ALLEGRO_SAMPLE *whichSfx)
{
    ALLEGRO_SAMPLE_INSTANCE *sfx;
    sfx = al_create_sample_instance(whichSfx);
    al_set_sample_instance_gain(sfx, Audio::sfxGain);

    al_attach_sample_instance_to_mixer(sfx, al_get_default_mixer());
    Audio::activeSfxInstances.push_back(sfx);
    al_play_sample_instance(sfx);
}

void Audio::SetActiveBgm(ALLEGRO_SAMPLE *whichBgm, unsigned int position_secs)
{
    if (activeBgmInstance != nullptr)
    {
        al_detach_sample_instance(activeBgmInstance);
        al_destroy_sample_instance(activeBgmInstance);
    }

    activeBgmInstance = al_create_sample_instance(whichBgm);
    al_set_sample_instance_playmode(activeBgmInstance, ALLEGRO_PLAYMODE_LOOP);
    al_set_sample_instance_gain(activeBgmInstance, bgmGain);
    al_set_sample_instance_position(activeBgmInstance, al_get_sample_instance_frequency(activeBgmInstance) * position_secs);
    al_attach_sample_instance_to_mixer(activeBgmInstance, al_get_default_mixer());
    al_play_sample_instance(activeBgmInstance);
}

void Audio::Update()
{
    for (std::vector<ALLEGRO_SAMPLE_INSTANCE *>::iterator it = activeSfxInstances.begin(); it != activeSfxInstances.end();)
    {
        if (!al_get_sample_instance_playing(*it))
        {
            al_detach_sample_instance(*it);
            al_destroy_sample_instance(*it);
            it = activeSfxInstances.erase(it);
        }
        else
        {
            ++it;
        }
    }
}
