#pragma once

#include <allegro5/allegro.h>

#include <sstream>

struct Configuration
{
    static ALLEGRO_CONFIG *settingsCfg;

    static void LoadConfigurations();
    static void UnloadConfigurations();

    static std::string GetString(const ALLEGRO_CONFIG *config, const char *section, const char *key);
    static int GetInt(const ALLEGRO_CONFIG *config, const char *section, const char *key);
    static void SetKey(ALLEGRO_CONFIG *config, const char *section, const char *key, const char *value);
    static void SetKey(ALLEGRO_CONFIG *config, const char *section, const char *key, int value);
};
