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

#include <iostream>
#include <sstream>
#include <string>
#include <thread>

extern IAIBackend* gAIBackend;

namespace QUADRAQ {

    void QUADRAQ_CLI_AISwitchLoop();

    // Main CLI control loop for QUADRAQ
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
                if (gAIBackend) {
                    std::cout << "[CLI] Current AI reports: " << gAIBackend->GetStatusString() << "\n";
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
                    gAIBackend = ai;
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

}  // namespace QUADRAQ