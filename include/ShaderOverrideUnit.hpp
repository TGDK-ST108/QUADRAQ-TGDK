#ifndef TGDK_SHADER_OVERRIDE_UNIT_HPP
#define TGDK_SHADER_OVERRIDE_UNIT_HPP

#include <d3d11.h>

namespace ShaderOverrideUnit {
    // Hooks into D3D pipeline
    bool HookPipeline(ID3D11Device* device, ID3D11DeviceContext* context);

    // Overrides current pixel shader
    void OverridePixelShader(ID3D11PixelShader* newShader);

    // Clears all shader overrides
    void ClearOverrides();

    // Called every frame to monitor GPU/render activity
    void FrameMonitor();

    // Enables/disables the override system
    void SetEnabled(bool enable);

    // Returns whether override system is currently active
    bool IsEnabled();
}

#endif // TGDK_SHADER_OVERRIDE_UNIT_HPP
