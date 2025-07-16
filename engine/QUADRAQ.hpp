// ====================================================================
//                          QUADRAQ.hpp
//     TGDK Quantum GPU Accelerator - Phase 1 Header
//     License: TGDK BFE-TGDK-022ST • Sean Tichenor • All Rights Reserved
// ====================================================================

#pragma once
#include <Windows.h>
#include "TGDK_IAIBackend.hpp"

namespace QUADRAQ {

    // Initializes QUADRAQ systems including AI, Shader Hooks, and Interceptors.
    DWORD WINAPI MainThreadProc(LPVOID);
    extern class IAIBackend* gAIBackendPtr;

} // namespace QUADRAQ
