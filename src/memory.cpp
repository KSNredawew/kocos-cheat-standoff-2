#include "offsets.hpp"
#include <sys/mman.h>
#include <unistd.h>
#include <android/log.h>

#define LOG_TAG "MEMHACK"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)

namespace Memory {
    // Get process base address
    uintptr_t GetBaseAddress(const char* module) {
        FILE* maps = fopen("/proc/self/maps", "r");
        char line[256];
        uintptr_t base = 0;

        while (fgets(line, sizeof(line), maps)) {
            if (strstr(line, module)) {
                sscanf(line, "%lx", &base);
                break;
            }
        }
        fclose(maps);
        return base;
    }

    // Read memory wrapper
    template <typename T>
    T Read(uintptr_t address) {
        if (address < 0x10000) return T();
        return *reinterpret_cast<T*>(address);
    }

    // Write memory wrapper
    template <typename T>
    void Write(uintptr_t address, T value) {
        if (address < 0x10000) return;
        
        long pageSize = sysconf(_SC_PAGESIZE);
        uintptr_t aligned = address & ~(pageSize - 1);
        mprotect((void*)aligned, pageSize, PROT_READ | PROT_WRITE | PROT_EXEC);
        
        *reinterpret_cast<T*>(address) = value;
    }

    // Read player position
    Vector3 ReadPosition(uintptr_t playerPtr) {
        return Read<Vector3>(playerPtr + Offsets::PLAYER_POSITION);
    }

    // Read bone position (0-18 bones)
    Vector3 ReadBonePosition(uintptr_t playerPtr, int boneId) {
        uintptr_t boneArray = Read<uintptr_t>(playerPtr + Offsets::PLAYER_BONE_ARRAY);
        return Read<Vector3>(boneArray + boneId * 0x30);
    }

    // World to screen transformation
    bool WorldToScreen(Vector3 world, Vector2& screen) {
        Matrix4x4 matrix = Read<Matrix4x4>(Offsets::GAME_BASE + Offsets::VIEW_MATRIX);
        
        // Matrix transformation math
        float w = matrix.m[3][0] * world.x + matrix.m[3][1] * world.y + matrix.m[3][2] * world.z + matrix.m[3][3];
        if (w < 0.001f) return false;
        
        screen.x = (matrix.m[0][0] * world.x + matrix.m[0][1] * world.y + matrix.m[0][2] * world.z + matrix.m[0][3]) / w;
        screen.y = (matrix.m[1][0] * world.x + matrix.m[1][1] * world.y + matrix.m[1][2] * world.z + matrix.m[1][3]) / w;
        
        // Convert to screen coordinates
        screen.x = (screen.x + 1.0f) * 0.5f * screenWidth;
        screen.y = (1.0f - screen.y) * 0.5f * screenHeight;
        
        return true;
    }
}
