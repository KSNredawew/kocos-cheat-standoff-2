#pragma once
#include <cstdint>

namespace Offsets {
    // Game version: Standoff 2 v0.33.3
    const uintptr_t GAME_BASE = 0x6A8F0000;
    
    // Player and entity system
    const uintptr_t ENTITY_LIST = 0x1A7F3C0;
    const uintptr_t LOCAL_PLAYER = 0x2C84F10;
    const uintptr_t PLAYER_COUNT = 0x1B3DA14;
    
    // Player structure
    const uintptr_t PLAYER_HEALTH = 0x1AC;
    const uintptr_t PLAYER_TEAM = 0x1B8;
    const uintptr_t PLAYER_POSITION = 0x1D4;
    const uintptr_t PLAYER_BONE_ARRAY = 0x2E4;
    
    // Weapon properties
    const uintptr_t CURRENT_WEAPON = 0x2B84A08;
    const uintptr_t AMMO = 0x2B84A10;
    const uintptr_t RECOIL = 0x1C88F24;
    const uintptr_t SPREAD = 0x1D99F38;
    
    // World to screen matrix
    const uintptr_t VIEW_MATRIX = 0x2C44D74;
    
    // Function pointers
    const uintptr_t FIRE_FN = 0x3A8F40;
    const uintptr_t AIM_FN = 0x3B8C20;
    
    // Guardian Anti-Cheat
    const uintptr_t GUARDIAN_SCAN = 0x5C8F00;
    const uintptr_t GUARDIAN_REPORT = 0x6A8F24;
}
