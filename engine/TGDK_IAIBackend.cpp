// ====================================================================
//                    TGDK_IAIBackend.cpp
//      Global AI backend for TGDK Quantum Stack (Factory Version)
//      License: TGDK BFE-TGDK-022ST • Sean Tichenor • All Rights Reserved
// ====================================================================

#include "TGDK_IAIBackend.hpp"
#include <iostream>
#include <memory>
#include <windows.h>

#ifdef TGDK_USE_OLIVIA
#include "OliviaAI.hpp"
#endif

// === Internal flag ===
static bool usingOlivia = false;

// === Global pointer instance ===
std::unique_ptr<IAIBackend> gAIBackend;
IAIBackend* gAIBackendPtr = nullptr;

// === Olivia Adapter (if enabled) ===
#ifdef TGDK_USE_OLIVIA
class OliviaBridgeAI : public IAIBackend {
public:
    bool Initialize() override {
        OliviaAI::Init();
        return true;
    }

    void Log(const std::string& msg) override {
        OliviaAI::Log(msg);
    }

    void LogError(const std::string& msg) override {
        OliviaAI::LogError(msg);
    }

    void OnFrame() override {
        OliviaAI::Pulse();
    }

    bool IsOliviaActive() const override {
        return true;
    }

    bool ShouldSuppressDraw(float) override {
        return OliviaAI::ShouldSkipDraw();
    }

    std::string GetBackendName() const override {
        return "OliviaAI";
    }
};
#endif

/*// === Factory Function ===
std::unique_ptr<IAIBackend> CreateAIBackend(bool useOlivia) {
#ifdef TGDK_USE_OLIVIA
    if (useOlivia) {
        usingOlivia = true;
        return std::make_unique<OliviaBridgeAI>();
    }
#endif
    usingOlivia = false;
    return std::make_unique<DefaultConsoleAI>();
}*/

// === Public Control Functions ===
/*    gAIBackend = CreateAIBackend(useOliviaFlag);
    gAIBackendPtr = gAIBackend.get();
    gAIBackend->Log(useOliviaFlag ? "Switched to OliviaAI." : "Switched to DefaultConsoleAI.");
}*/

bool IsOliviaActive() {
    return usingOlivia;
}

// === DLL Backend Loader ===
typedef IAIBackend* (*CreateAIBackendFunc)();

static HMODULE g_aiDllHandle = nullptr;
static IAIBackend* g_loadedAI = nullptr;

bool LoadAIBackendDLL(const std::string& dllPath) {
    g_aiDllHandle = LoadLibraryA(dllPath.c_str());
    if (!g_aiDllHandle) {
        std::cerr << "[TGDK] Failed to load DLL: " << dllPath << std::endl;
        return false;
    }

    auto createFunc = (CreateAIBackendFunc)GetProcAddress(g_aiDllHandle, "CreateAIBackend");
    if (!createFunc) {
        std::cerr << "[TGDK] Missing symbol: CreateAIBackend in " << dllPath << std::endl;
        return false;
    }

    g_loadedAI = createFunc();
    return g_loadedAI != nullptr;
}

IAIBackend* GetAIBackend() {
    return g_loadedAI ? g_loadedAI : gAIBackendPtr;
}
