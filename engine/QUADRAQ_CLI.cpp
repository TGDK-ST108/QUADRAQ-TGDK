// ====================================================================
//                         QUADRAQ_CLI.cpp
//      CLI Controller for TGDK Quantum GPU Accelerator - Phase 1
//      License: TGDK BFE-TGDK-022ST • Sean Tichenor • All Rights Reserved
// ====================================================================

#include "QUADRAQ.hpp"
#include "EntropyPredictor.hpp"
#include "ShaderOverrideUnit.hpp"
#include "QuantumDrawRouter.hpp"
#include "FlatDDSInterceptor.hpp"
#include "TGDK_IAIBackend.hpp"
#include "AIRegistry.hpp"
#include "KerflumpInterceptor.hpp"
#include "QUADRAQ_CLI.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <memory>
#include <d3d11.h>

// ==== BACKEND DECLARATIONS ==== //
#ifdef TGDK_USE_OLIVIA
#include "AI_Backends/OliviaAI.hpp"
#endif

#ifdef TGDK_USE_MARA
#include "AI_Backends/MaraAI.hpp"
#endif

#ifdef TGDK_USE_SHODAN
#include "AI_Backends/ShodanAI.hpp"
#endif

#ifdef TGDK_USE_STUB
#include "StubAI.hpp"
#endif

namespace QUADRAQ {
    extern IAIBackend* gAIBackendPtr;
}

std::unique_ptr<IAIBackend> TryInstantiateBackend() {
#ifdef TGDK_USE_OLIVIA
    try {
        auto ptr = std::make_unique<OliviaAI>();
        if (ptr->Initialize()) {
            std::cout << "[QUADRAQ] OliviaAI active.\n";
            return ptr;
        }
    }
    catch (...) {
        std::cerr << "[QUADRAQ] OliviaAI instantiation failed.\n";
    }
#endif

#ifdef TGDK_USE_MARA
    try {
        auto ptr = std::make_unique<MaraAI>();
        if (ptr->Initialize()) {
            std::cout << "[QUADRAQ] MaraAI active.\n";
            return ptr;
        }
    }
    catch (...) {
        std::cerr << "[QUADRAQ] MaraAI instantiation failed.\n";
    }
#endif

#ifdef TGDK_USE_SHODAN
    try {
        auto ptr = std::make_unique<ShodanAI>();
        if (ptr->Initialize()) {
            std::cout << "[QUADRAQ] ShodanAI active.\n";
            return ptr;
        }
    }
    catch (...) {
        std::cerr << "[QUADRAQ] ShodanAI instantiation failed.\n";
    }
#endif

#ifdef TGDK_USE_STUB
    std::cout << "[QUADRAQ] Falling back to StubAI.\n";
    return std::make_unique<StubAI>();
#else
    std::cerr << "[QUADRAQ] No valid backend available.\n";
    return nullptr;
#endif
}

// ==== CLI LOGIC ==== //
namespace QUADRAQ {
    IAIBackend* gAIBackendPtr = nullptr;
    ID3D11Device* g_device = nullptr;

    void QUADRAQ_CLI_AISwitchLoop();

    void QUADRAQ_CLI_Main() {
        bool entropyOn = true;
        bool shaderOn = true;
        bool routerOn = true;
        std::string input;

        while (true) {
            std::cout << "\n====== QUADRAQ :: CLI Control ======\n";
            std::cout << "[1] Toggle Entropy Predictor\n";
            std::cout << "[2] Toggle Shader Override\n";
            std::cout << "[3] Toggle Draw Router\n";
            std::cout << "[4] Reload Flat Texture\n";
            std::cout << "[5] View Current AI Status\n";
            std::cout << "[6] Enter AI Command Console\n";
            std::cout << "[0] Exit CLI\n";
            std::cout << "====================================\n";
            std::cout << "Enter selection: ";

            std::getline(std::cin, input);

            if (input == "1") {
                entropyOn = !entropyOn;
                EntropyPredictor::SetEnabled(entropyOn);
                std::cout << "[CLI] Entropy Predictor is now " << (entropyOn ? "ON" : "OFF") << "\n";
            }
            else if (input == "2") {
                shaderOn = !shaderOn;
                ShaderOverrideUnit::SetEnabled(shaderOn);
                std::cout << "[CLI] Shader Override is now " << (shaderOn ? "ON" : "OFF") << "\n";
            }
            else if (input == "3") {
                routerOn = !routerOn;
                QuantumDrawRouter::EnableRouting(routerOn);
                std::cout << "[CLI] Quantum Draw Router is now " << (routerOn ? "ON" : "OFF") << "\n";
            }
            else if (input == "4") {
                FlatDDSInterceptor::Clear();
                FlatDDSInterceptor::LoadFlatTexture(g_device, L"texture_flat_grayscale_minimal.dds");
                FlatDDSInterceptor::RegisterOverride("clouds_volumetric_layer.dds");
                FlatDDSInterceptor::RegisterOverride("atmosphere_blur_cube.dds");
                FlatDDSInterceptor::RegisterOverride("shader_fog_gradient.dds");
                FlatDDSInterceptor::RegisterOverride("motion_blur_occlusion.dds");
                FlatDDSInterceptor::RegisterOverride("volumetric_godrays.dds");
                FlatDDSInterceptor::RegisterOverride("low_quality_cloudlayer.dds");
                std::cout << "[CLI] Flat texture overrides reloaded.\n";
            }
            else if (input == "5") {
                if (gAIBackendPtr) {
                    std::cout << "[CLI] Current AI reports: " << gAIBackendPtr->GetStatusString() << "\n";
                }
                else {
                    std::cout << "[CLI] No AI backend active.\n";
                }
            }
            else if (input == "6") {
                QUADRAQ_CLI_AISwitchLoop();
            }
            else if (input == "0") {
                std::cout << "[CLI] QUADRAQ CLI shutdown requested.\n";
                break;
            }
            else {
                std::cout << "[CLI] Invalid input.\n";
            }
        }
    }

    void QUADRAQ_CLI_AISwitchLoop() {
        std::string command;

        std::cout << "\n=== QUADRAQ :: AI Console Mode ===\n";
        std::cout << "Commands:\n"
            << "  ai_register <label> <dll>\n"
            << "  ai_use <label>\n"
            << "  ai_list\n"
            << "  ai_unregister_all\n"
            << "  exit\n";

        while (true) {
            std::cout << "\nAI> ";
            std::getline(std::cin, command);

            if (command == "exit" || command == "quit") {
                std::cout << "Exiting AI Console...\n";
                break;
            }
            else if (command == "ai_list") {
                AIRegistry::PrintRegistered();
            }
            else if (command.rfind("ai_register ", 0) == 0) {
                std::istringstream ss(command.substr(12));
                std::string label, dllPath;
                ss >> label >> dllPath;
                if (!AIRegistry::RegisterAI(label, dllPath)) {
                    std::cout << "[CLI] Registration failed.\n";
                }
                else {
                    std::cout << "[CLI] AI '" << label << "' registered successfully.\n";
                }
            }
            else if (command.rfind("ai_use ", 0) == 0) {
                std::string label = command.substr(7);
                auto* ai = AIRegistry::Get(label);
                if (ai) {
                    gAIBackendPtr = ai;
                    ai->Log("Activated via CLI.");
                    std::cout << "[CLI] AI '" << label << "' is now active.\n";
                }
                else {
                    std::cout << "[CLI] AI '" << label << "' not found.\n";
                }
            }
            else if (command == "ai_unregister_all") {
                AIRegistry::Clear();
                std::cout << "[CLI] All AI modules unloaded.\n";
            }
            else {
                std::cout << "Unknown command. Type 'exit' to return.\n";
            }
        }
    }

} // namespace QUADRAQ

// ==== MAIN ENTRY POINT ==== //
int main() {
    std::cout << "\n==== QUADRAQ INIT PHASE ====\n";

    auto ai = TryInstantiateBackend();
    if (!ai) {
        std::cerr << "Fatal: No usable AI backend. Aborting.\n";
        return 1;
    }

    QUADRAQ::gAIBackendPtr = ai.get();

    // Optional: Initialize Kerflump Interceptor
    KerflumpInterceptor::Initialize(nullptr, nullptr);  // Replace with actual D3D device/context if needed

    std::cout << "\n[QUADRAQ] Running Warmup Cycle...\n";

    for (int i = 0; i < 5; ++i) {
        KerflumpInterceptor::PreFrameFold();
        QUADRAQ::gAIBackendPtr->OnFrameStart();
        QUADRAQ::gAIBackendPtr->OnFrame();
        QUADRAQ::gAIBackendPtr->OnFrameEnd();
        KerflumpInterceptor::PostFrameJumpFlush();
    }

    QUADRAQ::gAIBackendPtr->Shutdown();

    std::cout << "\n==== QUADRAQ CLI ====\n";
    QUADRAQ::QUADRAQ_CLI_Main();

    return 0;
}
