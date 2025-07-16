#include "ShaderOverrideUnit.hpp"
#include "TGDK_IAIBackend.hpp"

#include <d3d11.h>
#include <mutex>
#include <vector>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace ShaderOverrideUnit {

    static ID3D11Device* g_device = nullptr;
    static ID3D11DeviceContext* g_context = nullptr;
    static std::mutex shader_mutex;
    static std::mutex stateMutex;

    static bool hookInitialized = false;
    static bool overrideEnabled = false;
    static std::vector<ID3D11PixelShader*> overriddenShaders;


    bool HookPipeline(ID3D11Device* device, ID3D11DeviceContext* context) {
        std::lock_guard<std::mutex> lock(shader_mutex);

        if (hookInitialized)
            return true;

        g_device = device;
        g_context = context;

        if (!g_device || !g_context) {
            if (gAIBackendPtr) gAIBackendPtr->LogError("ShaderOverrideUnit :: Invalid device or context.");
            return false;
        }

        hookInitialized = true;
        if (gAIBackendPtr) gAIBackendPtr->Log("ShaderOverrideUnit :: HookPipeline successful.");
        return true;
    }

    void SetEnabled(bool enable) {
        std::lock_guard<std::mutex> lock(stateMutex);
        overrideEnabled = enable;

        if (enable) {
            if (!hookInitialized && (!g_device || !g_context)) {
                if (gAIBackendPtr) gAIBackendPtr->LogError("ShaderOverrideUnit :: Cannot enable - pipeline not hooked.");
                return;
            }

            if (gAIBackendPtr)
                gAIBackendPtr->Log("ShaderOverrideUnit :: Shader override ENABLED.");
        }
        else {
            ClearOverrides();

            if (gAIBackendPtr)
                gAIBackendPtr->Log("ShaderOverrideUnit :: Shader override DISABLED.");
        }

        // Optional: Trigger GPU state update or rebind shaders
    }

    bool IsEnabled() {
        std::lock_guard<std::mutex> lock(stateMutex);
        return overrideEnabled;
    }

    void OverridePixelShader(ID3D11PixelShader* newShader) {
        std::lock_guard<std::mutex> lock(shader_mutex);

        if (!g_context || !newShader) return;

        g_context->PSSetShader(newShader, nullptr, 0);
        overriddenShaders.push_back(newShader);

        if (gAIBackendPtr)
            gAIBackendPtr->Log("ShaderOverrideUnit :: Custom pixel shader applied.");
    }

    void ClearOverrides() {
        std::lock_guard<std::mutex> lock(shader_mutex);

        for (auto shader : overriddenShaders) {
            if (shader) shader->Release();
        }

        overriddenShaders.clear();

        if (gAIBackendPtr)
            gAIBackendPtr->Log("ShaderOverrideUnit :: Cleared all shader overrides.");
    }

    void FrameMonitor() {
        std::lock_guard<std::mutex> lock(shader_mutex);

        if (!g_context) return;

        // Optionally inspect or track per-frame shader state
    }
}
