// ====================================================================
//                        QuantumDrawRouter.cpp
//     TGDK Quantum GPU Accelerator — Render Path Interceptor
//     Reroutes or suppresses draw calls based on AI logic or entropy
//     Part of QUADRAQ BFE-TGDK-022ST
// ====================================================================

#include "QuantumDrawRouter.hpp"
#include "TGDK_IAIBackend.hpp"
#include "EntropyPredictor.hpp"
#include "QUADRAQ.hpp"

#include <d3d11.h>
#include <mutex>

namespace QuantumDrawRouter {
    static IAIBackend* gAIBackendPtr = nullptr;

    static bool routingEnabled = true;
    static float entropyThreshold = 0.010f; // Default drop level
    static std::mutex routerMutex;

    void EnableRouting(bool enable) {
        std::lock_guard<std::mutex> lock(routerMutex);
        routingEnabled = enable;

        IAIBackend* backend = GetAIBackend();
        if (backend) {
            backend->Log(std::string("QuantumDrawRouter :: Routing ") +
                (enable ? "ENABLED" : "DISABLED"));
        }
    }


    void SetEntropyThreshold(float threshold) {
        std::lock_guard<std::mutex> lock(routerMutex);
        entropyThreshold = threshold;

        if (gAIBackendPtr) {
            gAIBackendPtr->Log("QuantumDrawRouter :: Set entropy threshold to " + std::to_string(threshold));
        }
    }

    bool ShouldSuppressDraw() {
        std::lock_guard<std::mutex> lock(routerMutex);

        if (!routingEnabled)
            return false;

        float entropy = EntropyPredictor::GetCurrentEntropyRate();

        // Let AI override suppression decision
        if (gAIBackendPtr && gAIBackendPtr->ShouldSuppressDraw(entropy)) {
            return true;
        }

        return entropy > entropyThreshold;
    }

    void AttemptDraw(ID3D11DeviceContext* context, ID3D11Buffer* vertexBuffer, UINT stride, UINT offset) {
        if (ShouldSuppressDraw()) {
            if (gAIBackendPtr)
                gAIBackendPtr->LogError("QuantumDrawRouter :: Suppressed draw call due to high entropy.");
            return; // Skip rendering
        }

        // Basic forward example
        if (context && vertexBuffer) {
            UINT zero = 0;
            context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
            // TODO: Add draw call or additional logic here as needed
        }
    }
}