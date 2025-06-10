#include "offsets.hpp"
#include <substrate.h>
#include <android/log.h>

#define LOG_TAG "HOOKS"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

// Hook function pointers
void (*orig_FireWeapon)(void* weapon, Vector3 target);
void (*orig_AimAtPosition)(void* player, Vector3 position);

// Global cheat states
bool aimbotEnabled = true;
bool triggerbotEnabled = false;
float aimbotFOV = 45.0f;

// Hooked fire function
void hooked_FireWeapon(void* weapon, Vector3 target) {
    if (triggerbotEnabled) {
        // Auto-shoot when crosshair near enemy
        Vector3 currentAim = GetCurrentAimPosition();
        if (Distance(currentAim, target) < aimbotFOV / 2) {
            orig_FireWeapon(weapon, target);
        }
    } else {
        orig_FireWeapon(weapon, target);
    }
}

// Hooked aim function
void hooked_AimAtPosition(void* player, Vector3 position) {
    if (aimbotEnabled) {
        uintptr_t localPlayer = Memory::Read<uintptr_t>(Offsets::GAME_BASE + Offsets::LOCAL_PLAYER);
        Vector3 headPos = Memory::ReadBonePosition(localPlayer, 8); // Head bone
        
        // Find closest enemy to crosshair
        Vector3 closestTarget = FindClosestEnemy(headPos);
        if (Distance(headPos, closestTarget) < aimbotFOV) {
            orig_AimAtPosition(player, closestTarget);
            return;
        }
    }
    orig_AimAtPosition(player, position);
}

// Apply all hooks
void InstallHooks() {
    MSHookFunction(
        (void*)(Offsets::GAME_BASE + Offsets::FIRE_FN),
        (void*)hooked_FireWeapon,
        (void**)&orig_FireWeapon
    );
    
    MSHookFunction(
        (void*)(Offsets::GAME_BASE + Offsets::AIM_FN),
        (void*)hooked_AimAtPosition,
        (void**)&orig_AimAtPosition
    );
    
    // Patch recoil and spread
    Memory::Write<float>(Offsets::GAME_BASE + Offsets::RECOIL, 0.0f);
    Memory::Write<float>(Offsets::GAME_BASE + Offsets::SPREAD, 0.0f);
    
    LOGD("Hooks installed successfully!");
}

// Anti-cheat bypass
void BypassGuardian() {
    // Fake return true for cheat detection scans
    MSHookFunction(
        (void*)(Offsets::GAME_BASE + Offsets::GUARDIAN_SCAN),
        (void*)+[]() { return true; },
        nullptr
    );
    
    // Nullify report function
    MSHookFunction(
        (void*)(Offsets::GAME_BASE + Offsets::GUARDIAN_REPORT),
        (void*)+[](const char*) {},
        nullptr
    );
}
