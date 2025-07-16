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

#include <Windows.h>
#include <d3d11.h>
#include <thread>
#include <mutex>
#include <cstdlib>
#include "AIRegistry.hpp"

// Declare the global AI backend pointer
IAIBackend* gAIBackend = nullptr;

// During startup:
bool reg1 = AIRegistry::Register("olivia", "OliviaAI.dll");
bool reg2 = AIRegistry::Register("mara", "MaraAI.dll");
bool reg3 = AIRegistry::Register("shodan", "ShodanAI.dll");

using namespace QUADRAQ;

ID3D11Device* g_device = nullptr;
ID3D11DeviceContext* g_context = nullptr;
std::mutex quadraq_mutex;

bool initialized = false;
std::thread cliThread;

// ====================================================================
//                        AI DLL Loader
// ====================================================================

#include "TGDK_IAIBackend.hpp" // Ensure this is included for gAIBackend

bool LoadAIBackendDLL(const std::string& dllName) {
    // Unload previous DLL if needed (not shown here)
    HMODULE hModule = LoadLibraryA(dllName.c_str());
    if (!hModule) return false;

    // Example: get the backend creation function from the DLL
    using CreateAIBackendFunc = IAIBackend* (*)();
    auto createFunc = reinterpret_cast<CreateAIBackendFunc>(GetProcAddress(hModule, "CreateAIBackend"));
    if (!createFunc) {
        FreeLibrary(hModule);
        return false;
    }

    // Set global pointer (assumes gAIBackend is declared elsewhere)
    gAIBackend = createFunc();
    return gAIBackend != nullptr;
}

// Explicitly specify the namespace if there are multiple LoadCustomAI overloads
bool ::LoadCustomAI() {
    std::string aiDll = "OliviaAI.dll";  // Default AI DLL

    const char* envOverride = std::getenv("QUADRAQ_AI_DLL");
    if (envOverride) aiDll = envOverride;

    if (!LoadAIBackendDLL(aiDll)) {
        MessageBoxA(0, ("Failed to load AI DLL: " + aiDll).c_str(), "QUADRAQ", MB_ICONERROR);
        return false;
    }
    // Use in loop:
    if (auto* ai = AIRegistry::Get("mara")) {
        ai->Log("Mara AI active.");
    }
    if (auto* ai = AIRegistry::Get("shodan")) {
        ai->Log("Shodan AI active.");
    }
    if (auto* ai = AIRegistry::Get("olivia")) {
        ai->Log("Olivia AI active.");
    }
    if (gAIBackend) {
        gAIBackend->Log("QUADRAQ :: AI Backend initialized with DLL: " + aiDll);
    } else {
        MessageBoxA(0, "Failed to initialize AI backend", "QUADRAQ", MB_ICONERROR);
        return false;
    }
    if (!gAIBackend) {
        MessageBoxA(0, "No AI backend loaded", "QUADRAQ", MB_ICONERROR);
        return false;
    }
    if (!gAIBackend->Initialize()) {
        MessageBoxA(0, "AI backend initialization failed", "QUADRAQ", MB_ICONERROR);
        return false;
    }
    if (gAIBackend && gAIBackend->IsOliviaActive()) {
        gAIBackend->Log("QUADRAQ :: Using OliviaAI backend.");
    } else {
        gAIBackend->Log("QUADRAQ :: Using default AI backend.");
    }
    // Log the AI backend loading
    if (gAIBackend && gAIBackend->IsOliviaActive()) {
        gAIBackend->Log("QUADRAQ :: OliviaAI backend is active.");
    } else {
        gAIBackend->Log("QUADRAQ :: Default AI backend is active.");
    }
    if (gAIBackend)
        gAIBackend->Log("QUADRAQ :: Loaded AI Backend: " + aiDll);

    return true;
}

// ====================================================================
//                 QUADRAQ :: CLI Control Thread
// ====================================================================

void StartCLIRuntime() {
    cliThread = std::thread(QUADRAQ::QUADRAQ_CLI_Main);
    cliThread.detach();
}

// ====================================================================
//              QUADRAQ :: Flat Texture Interceptor Block
// ====================================================================

void InitFlatTextureIntercepts() {
    FlatDDSInterceptor::LoadFlatTexture(::g_device, L"texture_flat_grayscale_minimal.dds");

    ::FlatDDSInterceptor::RegisterOverride("clouds_volumetric_layer.dds");
    ::FlatDDSInterceptor::RegisterOverride("atmosphere_blur_cube.dds");
    ::FlatDDSInterceptor::RegisterOverride("shader_fog_gradient.dds");
    ::FlatDDSInterceptor::RegisterOverride("motion_blur_occlusion.dds");
    ::FlatDDSInterceptor::RegisterOverride("volumetric_godrays.dds");
    ::FlatDDSInterceptor::RegisterOverride("low_quality_cloudlayer.dds");

    if (gAIBackend)
        gAIBackend->Log("QUADRAQ :: FlatDDS overrides initialized.");
}

// ====================================================================
//                     QUADRAQ :: Main Init Thread
// ====================================================================

DWORD WINAPI ThreadFunc(LPVOID lpParam);

DWORD WINAPI QUADRAQ_MainThread(LPVOID lpParam) {
    if (!LoadCustomAI()) return 1;

    if (gAIBackend) gAIBackend->Log("QUADRAQ :: Initializing Quantum GPU Accelerator...");

    if (!EntropyPredictor::Initialize()) {
        if (gAIBackend) gAIBackend->LogError("QUADRAQ :: EntropyPredictor failed to init");
        return 1;
    }

    if (!ShaderOverrideUnit::HookPipeline(::g_device, ::g_context)) {
        if (gAIBackend) gAIBackend->LogError("QUADRAQ :: Failed to hook GPU pipeline");
        return 2;
    }

    ::InitFlatTextureIntercepts();
    QuantumDrawRouter::EnableRouting(true);

    if (gAIBackend) gAIBackend->Log("QUADRAQ :: Acceleration Layer Ready");
    ::initialized = true;

    ::StartCLIRuntime();

    while (true) {
        if (!::initialized) break;

        ShaderOverrideUnit::FrameMonitor();
        QuantumDrawRouter::ShouldSuppressDraw();
        EntropyPredictor::UpdateCycle();

        std::this_thread::sleep_for(std::chrono::milliseconds(4));
    }

    if (gAIBackend) gAIBackend->Log("QUADRAQ :: Shutting down...");
    return 0;
}

// ====================================================================
//                         QUADRAQ :: DllMain
// ====================================================================

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        // Explicitly cast to resolve overloaded-function ambiguity
        DWORD (WINAPI *pThreadFunc)(LPVOID) = static_cast<DWORD (WINAPI *)(LPVOID)>(&QUADRAQ_MainThread);
        HANDLE hThread = CreateThread(
            nullptr,         // lpThreadAttributes
            0,               // dwStackSize
            pThreadFunc,     // lpStartAddress
            nullptr,         // lpParameter
            0,               // dwCreationFlags
            nullptr          // lpThreadId
        );
        if (hThread) {
            CloseHandle(hThread); // Optionally close handle if not needed
        }
    }
    return TRUE;
}

// ====================================================================
//         QUADRAQ :: Optional Use in Render Hook Context
// ====================================================================
// ID3D11ShaderResourceView* replacementSRV = FlatDDSInterceptor::InterceptTexture(textureName);
// if (replacementSRV) {
//     g_context->PSSetShaderResources(slotIndex, 1, &replacementSRV);
//     if (gAIBackend) {
//         gAIBackend->Log("QUADRAQ :: Texture '" + textureName + "' overridden with flat minimal.");
//     }
// }