#ifndef TGDK_FLAT_DDS_INTERCEPTOR_HPP
#define TGDK_FLAT_DDS_INTERCEPTOR_HPP

#include <d3d11.h>
#include <string>

namespace FlatDDSInterceptor {
    bool LoadFlatTexture(ID3D11Device* device, const std::wstring& ddsPath);
    void RegisterOverride(const std::string& originalName);
    ID3D11ShaderResourceView* InterceptTexture(const std::string& textureName);
    void Clear();
}

#endif