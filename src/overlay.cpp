#include "offsets.hpp"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <imgui.h>
#include <imgui_impl_android.h>
#include <imgui_impl_opengl3.h>

// Rendering variables
ImVec2 screenSize = {1080, 2400};
bool menuVisible = true;

// Cheat settings
namespace Settings {
    bool espEnabled = true;
    bool espBox = true;
    bool espSkeleton = false;
    bool espHealth = true;
    
    bool aimbot = true;
    float aimbotFov = 45.0f;
    int aimbotBone = 8; // Head
    
    bool noRecoil = true;
    bool noSpread = true;
}

// Initialize overlay
void InitOverlay() {
    ImGui::CreateContext();
    ImGui_ImplAndroid_Init(nullptr);
    ImGui_ImplOpenGL3_Init("#version 300 es");
    
    // Style settings
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.1f, 0.1f, 0.1f, 0.9f);
}

// Render ESP elements
void RenderESP() {
    if (!Settings::espEnabled) return;
    
    uintptr_t gameBase = Memory::GetBaseAddress("libunity.so");
    uintptr_t localPlayer = Memory::Read<uintptr_t>(gameBase + Offsets::LOCAL_PLAYER);
    int playerCount = Memory::Read<int>(gameBase + Offsets::PLAYER_COUNT);
    uintptr_t* entities = Memory::Read<uintptr_t*>(gameBase + Offsets::ENTITY_LIST);
    
    for (int i = 0; i < playerCount; ++i) {
        uintptr_t player = entities[i];
        if (!player || player == localPlayer) continue;
        
        int health = Memory::Read<int>(player + Offsets::PLAYER_HEALTH);
        if (health <= 0) continue;
        
        // Get 2D screen position
        Vector3 headPos = Memory::ReadBonePosition(player, 8);
        Vector2 screenPos;
        if (!Memory::WorldToScreen(headPos, screenPos)) continue;
        
        // Render ESP elements
        if (Settings::espBox) {
            Vector3 footPos = Memory::ReadPosition(player);
            Vector2 footScreen;
            Memory::WorldToScreen(footPos, footScreen);
            
            float height = abs(screenPos.y - footScreen.y);
            float width = height / 2.4f;
            
            ImGui::GetBackgroundDrawList()->AddRect(
                ImVec2(screenPos.x - width/2, footScreen.y),
                ImVec2(screenPos.x + width/2, screenPos.y),
                IM_COL32(255, 0, 0, 255)
            );
        }
        
        if (Settings::espHealth) {
            ImGui::GetBackgroundDrawList()->AddText(
                ImVec2(screenPos.x, screenPos.y - 30),
                IM_COL32(0, 255, 0, 255),
                ("HP: " + std::to_string(health)).c_str()
            );
        }
    }
}

// Render cheat menu
void RenderMenu() {
    if (!menuVisible) return;
    
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame();
    ImGui::NewFrame();
    
    ImGui::Begin("Standoff 2 UltraHack v0.33.3", &menuVisible);
    
    // ESP Settings
    if (ImGui::CollapsingHeader("ESP Settings")) {
        ImGui::Checkbox("Enable ESP", &Settings::espEnabled);
        ImGui::Checkbox("Box ESP", &Settings::espBox);
        ImGui::Checkbox("Skeleton ESP", &Settings::espSkeleton);
        ImGui::Checkbox("Health Bar", &Settings::espHealth);
    }
    
    // Aimbot Settings
    if (ImGui::CollapsingHeader("Aimbot Settings")) {
        ImGui::Checkbox("Enable Aimbot", &Settings::aimbot);
        ImGui::SliderFloat("Aimbot FOV", &Settings::aimbotFov, 1.0f, 180.0f);
        ImGui::Combo("Target Bone", &Settings::aimbotBone, "Head\0Chest\0Pelvis\0");
    }
    
    // Weapon Mods
    if (ImGui::CollapsingHeader("Weapon Mods")) {
        ImGui::Checkbox("No Recoil", &Settings::noRecoil);
        ImGui::Checkbox("No Spread", &Settings::noSpread);
        if (ImGui::Button("Infinite Ammo")) {
            Memory::Write(gameBase + Offsets::AMMO, 999);
        }
    }
    
    ImGui::End();
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
