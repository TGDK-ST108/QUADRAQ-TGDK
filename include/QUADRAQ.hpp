// ====================================================================
//                          QUADRAQ.hpp
//     TGDK Quantum GPU Accelerator - Phase 1 Header
//     License: TGDK BFE-TGDK-022ST • Sean Tichenor • All Rights Reserved
// ====================================================================

#pragma once

#include <d3d11.h>
#include <string>
#include <mutex>
#include <thread>

namespace QUADRAQ {

    // Public globals
    extern ID3D11Device* g_device;
    extern ID3D11DeviceContext* g_context;
    extern std::mutex quadraq_mutex;

    // State flags
    extern bool initialized;

    // Initialization entry
    DWORD WINAPI QUADRAQ_MainThread(LPVOID lpParam);

    // CLI interface entry
    void StartCLIRuntime();

    // Texture handling
    void InitFlatTextureIntercepts();

    // AI Loading
    bool LoadCustomAI();

} // namespace QUADRAQ
