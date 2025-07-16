// ====================================================================
//                          QUADRAQ.cpp
//     TGDK Quantum GPU Accelerator - Phase 1 Initialization
//     License: TGDK BFE-TGDK-022ST • Sean Tichenor • All Rights Reserved
// ====================================================================

#include "QUADRAQ.hpp"
#include "QUADRAQ_CLI.hpp"
#include "ShaderOverrideUnit.hpp"
#include "EntropyPredictor.hpp"
#include "QuantumDrawRouter.hpp"
#include "FlatDDSInterceptor.hpp"
#include "TGDK_IAIBackend.hpp"
#include "AIRegistry.hpp"
#include "AI_Backends\OliviaAI.hpp"
#include "AI_Backends\MaraAI.hpp"
#include "AI_Backends\ShodanAI.hpp"
#include <iostream>
#include <windows.h>
#include <d3d11.h>
#include <chrono>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <unordered_map>
#include <Windows.h>
#include <d3d11.h>
#include <thread>
#include <mutex>
#include <cstdlib>

namespace QUADRAQ {

    // === Global State ===
    static IAIBackend* gAIBackend = nullptr;
    static ID3D11Device* g_device = nullptr;
    static ID3D11DeviceContext* g_context = nullptr;
    static std::mutex quadraq_mutex;
    static bool initialized = false;
    static std::thread cliThread;


    static void RegisterBackends() {
        AIRegistry::Register("olivia", std::make_unique<OliviaAI>());
        AIRegistry::Register("mara", std::make_unique<MaraAI>());
        AIRegistry::Register("shodan", std::make_unique<ShodanAI>());
    }

    static bool LoadAIBackendDLL(const std::string& dllName) {
        HMODULE hModule = LoadLibraryA(dllName.c_str());
        if (!hModule) return false;

        using CreateAIBackendFunc = IAIBackend * (*)();
        auto createFunc = reinterpret_cast<CreateAIBackendFunc>(GetProcAddress(hModule, "CreateAIBackend"));
        if (!createFunc) {
            FreeLibrary(hModule);
            return false;
        }

        gAIBackend = createFunc();
        return gAIBackend != nullptr;
    }

    static bool LoadCustomAI() {
        std::string aiDll = "OliviaAI.dll";
        if (const char* envOverride = std::getenv("QUADRAQ_AI_DLL")) {
            aiDll = envOverride;
        }

        RegisterBackends();

        if (!LoadAIBackendDLL(aiDll)) {
            MessageBoxA(nullptr, ("Failed to load AI DLL: " + aiDll).c_str(), "QUADRAQ", MB_ICONERROR);
            return false;
        }

        if (auto* ai = AIRegistry::Get("mara")) ai->Log("Mara AI active.");
        if (auto* ai = AIRegistry::Get("shodan")) ai->Log("Shodan AI active.");
        if (auto* ai = AIRegistry::Get("olivia")) ai->Log("Olivia AI active.");

        if (gAIBackend) {
            gAIBackend->Log("QUADRAQ :: AI Backend initialized with DLL: " + aiDll);
            if (!gAIBackend->Initialize()) {
                MessageBoxA(nullptr, "AI backend initialization failed", "QUADRAQ", MB_ICONERROR);
                return false;
            }

            if (gAIBackend->IsOliviaActive()) {
                gAIBackend->Log("QUADRAQ :: Using OliviaAI backend.");
            }
            else {
                gAIBackend->Log("QUADRAQ :: Using default AI backend.");
            }

            return true;
        }

        MessageBoxA(nullptr, "Failed to initialize AI backend", "QUADRAQ", MB_ICONERROR);
        return false;
    }

    static void StartCLIRuntime() {
        cliThread = std::thread(QUADRAQ_CLI_Main);
        cliThread.detach();
    }

    static void InitFlatTextureIntercepts() {
        FlatDDSInterceptor::LoadFlatTexture(g_device, L"texture_flat_grayscale_minimal.dds");

        FlatDDSInterceptor::RegisterOverride("clouds_volumetric_layer.dds");
        FlatDDSInterceptor::RegisterOverride("atmosphere_blur_cube.dds");
        FlatDDSInterceptor::RegisterOverride("shader_fog_gradient.dds");
        FlatDDSInterceptor::RegisterOverride("motion_blur_occlusion.dds");
        FlatDDSInterceptor::RegisterOverride("volumetric_godrays.dds");
        FlatDDSInterceptor::RegisterOverride("low_quality_cloudlayer.dds");

        if (gAIBackend)
            gAIBackend->Log("QUADRAQ :: FlatDDS overrides initialized.");
    }

    static DWORD WINAPI MainThreadProc(LPVOID) {
        if (!LoadCustomAI()) return 1;

        if (gAIBackend) gAIBackend->Log("QUADRAQ :: Initializing Quantum GPU Accelerator...");

        if (!EntropyPredictor::Initialize()) {
            if (gAIBackend) gAIBackend->LogError("QUADRAQ :: EntropyPredictor failed to init");
            return 1;
        }

        if (!ShaderOverrideUnit::HookPipeline(g_device, g_context)) {
            if (gAIBackend) gAIBackend->LogError("QUADRAQ :: Failed to hook GPU pipeline");
            return 2;
        }

        InitFlatTextureIntercepts();
        QuantumDrawRouter::EnableRouting(true);

        if (gAIBackend) gAIBackend->Log("QUADRAQ :: Acceleration Layer Ready");
        initialized = true;

        StartCLIRuntime();

        while (initialized) {
            ShaderOverrideUnit::FrameMonitor();
            QuantumDrawRouter::ShouldSuppressDraw();
            EntropyPredictor::UpdateCycle();

            std::this_thread::sleep_for(std::chrono::milliseconds(4));
        }

        if (gAIBackend) gAIBackend->Log("QUADRAQ :: Shutting down...");
        return 0;
    }

} // namespace QUADRAQ

// ====================================================================
//                         QUADRAQ :: DllMain
// ====================================================================

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        HANDLE hThread = CreateThread(
            nullptr,
            0,
            (LPTHREAD_START_ROUTINE)QUADRAQ::MainThreadProc,
            nullptr,
            0,
            nullptr
        );
        if (hThread) CloseHandle(hThread);
    }
    return TRUE;
}
