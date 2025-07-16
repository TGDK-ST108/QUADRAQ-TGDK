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

// === Global Backend Pointers ===
static std::unique_ptr<IAIBackend> g_ownedBackend = nullptr;
IAIBackend* gAIBackendPtr = nullptr;
static bool usingOlivia = false;

// === DLL Loader State ===
static HMODULE g_aiDllHandle = nullptr;

// === Olivia Adapter Implementation ===
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

    std::string Identify() const override { return "OliviaAI"; }
    std::string GetStatusString() const override { return "Live"; }
};
#endif

// === DLL Load Entry Point ===
typedef IAIBackend* (*CreateAIBackendFunc)();

bool LoadAIBackendDLL(const std::string& dllPath) {
    g_aiDllHandle = LoadLibraryA(dllPath.c_str());
    if (!g_aiDllHandle) {
        std::cerr << "[TGDK] Failed to load DLL: " << dllPath << std::endl;
        return false;
    }

    auto createFunc = (CreateAIBackendFunc)GetProcAddress(g_aiDllHandle, "CreateAIBackend");
    if (!createFunc) {
        std::cerr << "[TGDK] Missing symbol: CreateAIBackend in " << dllPath << std::endl;
        FreeLibrary(g_aiDllHandle);
        g_aiDllHandle = nullptr;
        return false;
    }

    SetAIBackend(std::unique_ptr<IAIBackend>(createFunc()));
    return gAIBackendPtr != nullptr;
}

// === Core Access ===
IAIBackend* GetAIBackend() {
    return gAIBackendPtr;
}

void SetAIBackend(std::unique_ptr<IAIBackend> backend) {
    g_ownedBackend = std::move(backend);
    gAIBackendPtr = g_ownedBackend.get();
}

void ClearAIBackend() {
    g_ownedBackend.reset();
    gAIBackendPtr = nullptr;

    if (g_aiDllHandle) {
        FreeLibrary(g_aiDllHandle);
        g_aiDllHandle = nullptr;
    }
}

bool IsOliviaActive() {
    return usingOlivia;
}

// === Optional Factory Call ===
std::unique_ptr<IAIBackend> CreateOliviaBridgeIfEnabled() {
#ifdef TGDK_USE_OLIVIA
    usingOlivia = true;
    return std::make_unique<OliviaBridgeAI>();
#else
    return nullptr;
#endif
}
