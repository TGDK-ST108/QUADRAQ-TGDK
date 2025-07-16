#ifndef TGDK_QUANTUM_DRAW_ROUTER_HPP
#define TGDK_QUANTUM_DRAW_ROUTER_HPP

#include <d3d11.h>

namespace QuantumDrawRouter {
    void EnableRouting(bool enable);
    void SetEntropyThreshold(float threshold);
    bool ShouldSuppressDraw();
    void AttemptDraw(ID3D11DeviceContext* context, ID3D11Buffer* vertexBuffer, UINT stride, UINT offset);
}

#endif