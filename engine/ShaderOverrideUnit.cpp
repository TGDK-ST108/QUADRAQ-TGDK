// ====================================================================
//                       ShaderOverrideUnit.cpp
//    TGDK Quantum GPU Accelerator — Shader Hook & Override Engine
//    Part of QUADRAQ BFE-TGDK-022ST
// ====================================================================

#include "ShaderOverrideUnit.hpp"
#include "TGDK_IAIBackend.hpp"
#include "QUADRAQ.hpp"

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>
#include <vector>
#include <mutex>
#include <string>

using namespace Microsoft::WRL;

namespace {
    struct Vertex {
        float position[3];
    };
}

namespace ShaderOverrideUnit {

    static IAIBackend* gAIBackendPtr = nullptr;
    static ID3D11Device* g_device = nullptr;
    static ID3D11DeviceContext* g_context = nullptr;
    static std::mutex shader_mutex;
    static std::vector<ID3D11PixelShader*> overriddenShaders;
    static bool hookInitialized = false;
    static bool overrideEnabled = false;
    static bool minimalMode = false;
    static std::mutex stateMutex;

    bool HookPipeline(ID3D11Device* device, ID3D11DeviceContext* context) {
        std::lock_guard<std::mutex> lock(shader_mutex);

        if (hookInitialized)
            return true;

        g_device = device;
        g_context = context;

        if (!g_device || !g_context) {
            if (gAIBackendPtr)
                gAIBackendPtr->LogError("ShaderOverrideUnit :: Invalid D3D device/context.");
            return false;
        }

        hookInitialized = true;

        if (gAIBackendPtr)
            gAIBackendPtr->Log("ShaderOverrideUnit :: HookPipeline successful.");

        return true;
    }

    void SetEnabled(bool enable) {
        std::lock_guard<std::mutex> lock(stateMutex);
        overrideEnabled = enable;

        if (enable) {
            if (!hookInitialized && (!g_device || !g_context)) {
                if (gAIBackendPtr)
                    gAIBackendPtr->LogError("ShaderOverrideUnit :: Cannot enable - pipeline not hooked.");
                return;
            }
            if (gAIBackendPtr)
                gAIBackendPtr->Log("ShaderOverrideUnit :: Shader overrides ENABLED.");
        }
        else {
            ClearOverrides();
            if (gAIBackendPtr)
                gAIBackendPtr->Log("ShaderOverrideUnit :: Shader overrides DISABLED.");
        }
    }

    bool IsEnabled() {
        std::lock_guard<std::mutex> lock(stateMutex);
        return overrideEnabled;
    }

    void ShaderOverrideUnit::ForceMinimal(bool state) {
        minimalMode = state;
    }

    void OverridePixelShader(ID3D11PixelShader* newShader) {
        std::lock_guard<std::mutex> lock(shader_mutex);

        if (!g_context || !newShader)
            return;

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

    // --- Moved out: Dummy draw to stimulate pipeline ---
    void IssueDummyDraw() {
        if (!g_device || !g_context) return;

        Vertex vertices[] = {
            { { -0.5f, -0.5f, 0.0f } },
            { {  0.5f, -0.5f, 0.0f } },
            { {  0.0f,  0.5f, 0.0f } }
        };

        D3D11_BUFFER_DESC bufferDesc = {};
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.ByteWidth = sizeof(vertices);
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

        D3D11_SUBRESOURCE_DATA initData = {};
        initData.pSysMem = vertices;

        ComPtr<ID3D11Buffer> vertexBuffer;
        HRESULT hr = g_device->CreateBuffer(&bufferDesc, &initData, vertexBuffer.GetAddressOf());
        if (FAILED(hr)) {
            if (gAIBackendPtr)
                gAIBackendPtr->LogError("ShaderOverrideUnit :: Failed to create dummy vertex buffer.");
            return;
        }

        UINT stride = sizeof(Vertex);
        UINT offset = 0;
        g_context->IASetVertexBuffers(0, 1, vertexBuffer.GetAddressOf(), &stride, &offset);
        g_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
        g_context->Draw(3, 0);
    }

    void FrameMonitor() {
        std::lock_guard<std::mutex> lock(shader_mutex);

        if (!g_context || !hookInitialized)
            return;

        ComPtr<ID3D11Query> pipelineQuery;
        D3D11_QUERY_DESC queryDesc = {};
        queryDesc.Query = D3D11_QUERY_PIPELINE_STATISTICS;

        HRESULT hr = g_device->CreateQuery(&queryDesc, pipelineQuery.GetAddressOf());
        if (FAILED(hr)) {
            if (gAIBackendPtr)
                gAIBackendPtr->LogError("ShaderOverrideUnit :: Failed to create pipeline query.");
            return;
        }

        g_context->Begin(pipelineQuery.Get());
        IssueDummyDraw();
        g_context->End(pipelineQuery.Get());

        while (g_context->GetData(pipelineQuery.Get(), nullptr, 0, 0) == S_FALSE) {
            // Spin until query is ready
        }

        D3D11_QUERY_DATA_PIPELINE_STATISTICS stats = {};
        if (SUCCEEDED(g_context->GetData(pipelineQuery.Get(), &stats, sizeof(stats), 0))) {
            uint64_t vsInvocations = stats.VSInvocations;
            uint64_t psInvocations = stats.PSInvocations;
            uint64_t rasterPrimitives = stats.CInvocations;

            std::string report =
                "[ShaderOverrideUnit] FrameMonitor :: " +
                std::to_string(vsInvocations) + " VS | " +
                std::to_string(psInvocations) + " PS | " +
                std::to_string(rasterPrimitives) + " Raster";

            if (gAIBackendPtr)
                gAIBackendPtr->Log(report);
        }
        else {
            if (gAIBackendPtr)
                gAIBackendPtr->LogError("ShaderOverrideUnit :: Failed to retrieve pipeline statistics.");
        }
    }

} // namespace ShaderOverrideUnit
