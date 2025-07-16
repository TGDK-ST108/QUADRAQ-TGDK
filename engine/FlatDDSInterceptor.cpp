// ====================================================================
//                      FlatDDSInterceptor.cpp
//     TGDK GPU Override — Replaces Volumetric & Blur Textures
//     Part of QUADRAQ BFE-TGDK-022ST
// ====================================================================

#include "FlatDDSInterceptor.hpp"
#include "TGDK_IAIBackend.hpp"
#include "QUADRAQ.hpp"

#include <d3d11.h>
#include <wrl.h>
#include <unordered_map>
#include <string>
#include <mutex>
#include <DDSTextureLoader.h>

using namespace Microsoft::WRL;

namespace FlatDDSInterceptor {

    static IAIBackend* gAIBackendPtr = nullptr;
    static ID3D11ShaderResourceView* flatTextureSRV = nullptr;
    static std::unordered_map<std::string, ID3D11ShaderResourceView*> redirectMap;
    static std::mutex interceptMutex;

    bool LoadFlatTexture(ID3D11Device* device, const std::wstring& ddsPath) {
        std::lock_guard<std::mutex> lock(interceptMutex);

        if (flatTextureSRV)
            return true;

        HRESULT hr = DirectX::CreateDDSTextureFromFile(device, ddsPath.c_str(), nullptr, &flatTextureSRV);
        if (FAILED(hr)) {
            if (gAIBackendPtr) gAIBackendPtr->LogError("FlatDDSInterceptor :: Failed to load flat DDS.");
            return false;
        }

        if (gAIBackendPtr)
            gAIBackendPtr->Log("FlatDDSInterceptor :: Flat DDS loaded from " + std::string(ddsPath.begin(), ddsPath.end()));

        return true;
    }

    void RegisterOverride(const std::string& originalName) {
        std::lock_guard<std::mutex> lock(interceptMutex);
        redirectMap[originalName] = flatTextureSRV;
    }

    ID3D11ShaderResourceView* InterceptTexture(const std::string& textureName) {
        std::lock_guard<std::mutex> lock(interceptMutex);
        auto it = redirectMap.find(textureName);
        if (it != redirectMap.end()) {
            if (gAIBackendPtr)
                gAIBackendPtr->Log("FlatDDSInterceptor :: Overriding texture: " + textureName);
            return it->second;
        }
        return nullptr;
    }

    void Clear() {
        std::lock_guard<std::mutex> lock(interceptMutex);
        if (flatTextureSRV) {
            flatTextureSRV->Release();
            flatTextureSRV = nullptr;
        }
        redirectMap.clear();

        if (gAIBackendPtr)
            gAIBackendPtr->Log("FlatDDSInterceptor :: Cleared overrides.");
    }
}