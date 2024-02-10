#pragma once
#include"structures.hpp"
#include"Driver.h"
#include "offsets.h"
#include"Config.hpp"
inline void modifyHighlights() {
    /*
    float SENSE_ENEMY_VISIBLE_COLOR_RED = Config::enargb.x * 10.f;
    float SENSE_ENEMY_VISIBLE_COLOR_GREEN = Config::enargb.y * 10.f;
    float SENSE_ENEMY_VISIBLE_COLOR_BLUE = Config::enargb.z * 10.f;
    int SENSE_ENEMY_VISIBLE_BODY_STYLE = 109;
    int SENSE_ENEMY_VISIBLE_BORDER_STYLE = 108;
    int SENSE_ENEMY_VISIBLE_BORDER_WIDTH = 60;

    float SENSE_ENEMY_INVISIBLE_COLOR_RED = Config::invisenargb.x * 10.f;
    float SENSE_ENEMY_INVISIBLE_COLOR_GREEN = Config::invisenargb.y * 10.f;
    float SENSE_ENEMY_INVISIBLE_COLOR_BLUE = Config::invisenargb.z * 10.f;
    int SENSE_ENEMY_INVISIBLE_BODY_STYLE = 2;//109;
    int SENSE_ENEMY_INVISIBLE_BORDER_STYLE = 108;
    int SENSE_ENEMY_INVISIBLE_BORDER_WIDTH = 45;

    float SENSE_ENEMY_LOCKEDON_COLOR_RED = 0;
    float SENSE_ENEMY_LOCKEDON_COLOR_GREEN = 0;
    float SENSE_ENEMY_LOCKEDON_COLOR_BLUE = 5;
    int SENSE_ENEMY_LOCKEDON_BODY_STYLE = 112;
    int SENSE_ENEMY_LOCKEDON_BORDER_STYLE = 4;
    int SENSE_ENEMY_LOCKEDON_BORDER_WIDTH = 45;

    const uintptr_t highlightSettingsPtr = read<uintptr_t>(oBaseAddress + OFFSET_GLOW_HIGHLIGHT_SETTINGS);
    const long highlightSize = 0x34;
    { //player highlight - visible 
        int highlightId = 0;
        const GlowMode newGlowMode = {
            SENSE_ENEMY_VISIBLE_BODY_STYLE,
            SENSE_ENEMY_VISIBLE_BORDER_STYLE,
            SENSE_ENEMY_VISIBLE_BORDER_WIDTH,
            127
        };
        const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
        if (newGlowMode != oldGlowMode)
            write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowMode);
        Color newColor = {
             SENSE_ENEMY_VISIBLE_COLOR_RED,
             SENSE_ENEMY_VISIBLE_COLOR_GREEN,
             SENSE_ENEMY_VISIBLE_COLOR_BLUE
        };
        const Color oldColor = read<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4);
        if (oldColor != newColor)
            write<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4, newColor);
    }
    { //player highlight - invisible
        int highlightId = 1;
        const GlowMode newGlowMode = {
             SENSE_ENEMY_INVISIBLE_BODY_STYLE,
             SENSE_ENEMY_INVISIBLE_BORDER_STYLE,
             SENSE_ENEMY_INVISIBLE_BORDER_WIDTH,
            100
        };
        const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
        if (newGlowMode != oldGlowMode)
            write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowMode);
        Color newColor = {
             SENSE_ENEMY_INVISIBLE_COLOR_RED,
             SENSE_ENEMY_INVISIBLE_COLOR_GREEN,
             SENSE_ENEMY_INVISIBLE_COLOR_BLUE
        };
        const Color oldColor = read<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4);
        if (oldColor != newColor)
            write<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4, newColor);
    }
    { //player highlight - teammates
        int highlightId = 2;
        const GlowMode newGlowMode = {
            // SENSE_ENEMY_LOCKEDON_BODY_STYLE,
            // SENSE_ENEMY_LOCKEDON_BORDER_STYLE,
             //SENSE_ENEMY_LOCKEDON_BORDER_WIDTH,
            //127
            0,0,0,0
        };
        const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
        if (newGlowMode != oldGlowMode)
            write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowMode);
        Color newColor = { SENSE_ENEMY_LOCKEDON_COLOR_RED, SENSE_ENEMY_LOCKEDON_COLOR_GREEN, SENSE_ENEMY_LOCKEDON_COLOR_BLUE };
        const Color oldColor = read<Color>(highlightSettingsPtr + (highlightSize * highlightId) +4);
        if (oldColor != newColor)
            write<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4, newColor);
    }

    const GlowMode newGlowModeShieldBased = { 2,113,80,127 };
    { //player highlight | shields = 0 
        int highlightId = 90;
        const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
        if (newGlowModeShieldBased != oldGlowMode)
            write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowModeShieldBased);
        Color newColor = { 0,10,0 };
        const Color oldColor = read<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4);
        if (oldColor != newColor)
            write<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4, newColor);
    }
    { //player highlight | shields <= 50  
        int highlightId = 91;
        const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
        if (newGlowModeShieldBased != oldGlowMode)
            write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowModeShieldBased);
        Color newColor = { 10,10,10 };
        const Color oldColor = read<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4);
        if (oldColor != newColor)
            write<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4, newColor);
    }
    { //player highlight | shields <= 75  
        int highlightId = 92;
        const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
        if (newGlowModeShieldBased != oldGlowMode)
            write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowModeShieldBased);
        Color newColor = { 0,2,10 };
        const Color oldColor = read<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4);
        if (oldColor != newColor)
            write<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4, newColor);
    }
    { //player highlight | shields <= 100  
        int highlightId = 93;
        const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
        if (newGlowModeShieldBased != oldGlowMode)
            write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowModeShieldBased);
        Color newColor = { 5,0,10 };
        const Color oldColor = read<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4);
        if (oldColor != newColor)
            write<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4, newColor);
    }
    { //player highlight | shields <= 125 
        int highlightId = 94;
        const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
        if (newGlowModeShieldBased != oldGlowMode)
            write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowModeShieldBased);
        Color newColor = { 10,1,0 };
        const Color oldColor = read<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4);
        if (oldColor != newColor)
            write<Color>(highlightSettingsPtr + (highlightSize * highlightId) + 4, newColor);
    }

    */

    //item highlights
    if (1) {
        const uintptr_t highlightSettingsPtr = read<uintptr_t>(oBaseAddress + OFFSET_GLOW_HIGHLIGHT_SETTINGS);
        const long highlightSize = 0x34;
        for (int highlightId = 1; highlightId < 50; highlightId++) {
            if (Config::ItemEsp) {
                const GlowMode newGlowMode = { 137,138,35,127 };
                const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
                if (newGlowMode != oldGlowMode)
                    write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowMode);
            }
            else {
                const GlowMode newGlowMode = { 135, 135,35,127 };
                const GlowMode oldGlowMode = read<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0);
                if (newGlowMode != oldGlowMode)
                    write<GlowMode>(highlightSettingsPtr + (highlightSize * highlightId) + 0, newGlowMode);
            }
        }
    }

    /* for (unsigned char i = 26; i < 44; i++) {
         write<unsigned char>(highlightSettingsPtr + 0x28 * (unsigned int)i + (unsigned int)4, (unsigned char)137);
         write<unsigned char>(highlightSettingsPtr + 0x28 * (unsigned int)i + (unsigned int)4 + sizeof(unsigned char), (unsigned char)138);
         write<unsigned char>(highlightSettingsPtr + 0x28 * (unsigned int)i + (unsigned int)4 + sizeof(unsigned char) * 2, (unsigned char)NULL);
         write<unsigned char>(highlightSettingsPtr + 0x28 * (unsigned int)i + (unsigned int)4 + sizeof(unsigned char) * 3, (unsigned char)NULL);
     }*/
}