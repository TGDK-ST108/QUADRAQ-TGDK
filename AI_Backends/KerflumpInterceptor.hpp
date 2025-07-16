// KerflumpInterceptor.hpp

#pragma once
#include <d3d11.h>

class KerflumpInterceptor {
public:
    static void Initialize(ID3D11Device* device, ID3D11DeviceContext* context);
    static void PreFrameFold();
    static void PostFrameJumpFlush();
    static void ForceUnderfoldTruncate();
    static void LogFrameState();
};
