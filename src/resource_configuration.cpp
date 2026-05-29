#include "resource_configuration.h"

#include <iostream>

ALLEGRO_CONFIG *Configuration::settingsCfg;

void Configuration::LoadConfigurations()
{
    settingsCfg = al_load_config_file("config/settings.cfg");
    if (!settingsCfg)
        std::cout << "Error - Configuration: config/settings.cfg not loaded." << std::endl;
    else
        std::cout << "Configuration: config/settings.cfg successfully loaded." << std::endl;

}

void Configuration::UnloadConfigurations()
{
    al_destroy_config(settingsCfg);
}

std::string Configuration::GetString(const ALLEGRO_CONFIG *config, const char *section, const char *key)
{
    const char *configValue = al_get_config_value(config, section, key);
    if (configValue != NULL)
    {
        std::string configValueString = configValue;

        // Replace any literal \n (two chars) in loaded text to an actual newline character.
        size_t pos = 0;
        while ((pos = configValueString.find("\\n", pos)) != std::string::npos)
        {
            configValueString.replace(pos, 2, "\n");
            pos++;
        }

        return configValueString;
    }
    else // configValue == NULL
        return "";
}

int Configuration::GetInt(const ALLEGRO_CONFIG *config, const char *section, const char *key)
{
    const char *configValue = al_get_config_value(config, section, key);
    if (configValue != NULL)
    {
        std::stringstream strValue;
        strValue << configValue;

        int intValue;
        strValue >> intValue;

        return intValue;
    }
    else
        return 0;
}

void Configuration::SetKey(ALLEGRO_CONFIG *config, const char *section, const char *key, const char *value)
{
    al_set_config_value(config, section, key, value);
}

void Configuration::SetKey(ALLEGRO_CONFIG *config, const char *section, const char *key, int value)
{
    al_set_config_value(config, section, key, std::to_string(value).c_str());
}
