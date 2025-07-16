#pragma once

namespace EntropyPredictor {
    bool Initialize();
    void SetEnabled(bool enable);
    bool IsEnabled();
    float GetCurrentEntropyRate();
    void UpdateCycle();
    bool IsOverloaded();
}
