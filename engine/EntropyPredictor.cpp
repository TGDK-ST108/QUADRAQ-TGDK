// ====================================================================
//                        EntropyPredictor.cpp
//     TGDK Quantum GPU Accelerator — Entropy Cycle Tracker
//     Predicts GPU stalls, scene transitions, and occlusion changes
//     Part of QUADRAQ BFE-TGDK-022ST
// ====================================================================

#include "EntropyPredictor.hpp"
#include "TGDK_IAIBackend.hpp"
#include <DDSTextureLoader.h>

#include <chrono>
#include <cmath>
#include <mutex>
#include <deque>


namespace EntropyPredictor {

    static IAIBackend* gAIBackendPtr = nullptr;
    static std::deque<float> frameTimes;
    static const size_t sampleWindow = 120; // Sliding window of last N frames
    static std::chrono::high_resolution_clock::time_point lastTime;
    static std::mutex entropyMutex;

    static float entropyRate = 0.0f;
    static bool initialized = false;
    static bool enabled = false;

    // Add a static variable to track overload state
    static bool overload = false;

    bool Initialize() {
        std::lock_guard<std::mutex> lock(entropyMutex);
        frameTimes.clear();
        lastTime = std::chrono::high_resolution_clock::now();
        entropyRate = 0.0f;
        initialized = true;

        if (GetAIBackend()) {
            GetAIBackend()->Log("Running");
        }

        return true; // Added return statement to fix C4716
    }

    void SetEnabled(bool enable) {
        std::lock_guard<std::mutex> lock(entropyMutex);
        enabled = enable;
        if (enable) {
            if (!initialized) Initialize();
            if (gAIBackendPtr) gAIBackendPtr->Log("EntropyPredictor :: Enabled");
        }
        else {
            frameTimes.clear();
            entropyRate = 0.0f;
            initialized = false;
            if (gAIBackendPtr) gAIBackendPtr->Log("EntropyPredictor :: Disabled");
        }
    }

    bool IsEnabled() {
        std::lock_guard<std::mutex> lock(entropyMutex);
        return enabled;
    }

    bool EntropyPredictor::IsOverloaded() {
        return overload;
    }

    float GetCurrentEntropyRate() {
        std::lock_guard<std::mutex> lock(entropyMutex);
        return entropyRate;
    }

    void UpdateCycle() {
        std::lock_guard<std::mutex> lock(entropyMutex);
        if (!initialized || !enabled) return;

        auto now = std::chrono::high_resolution_clock::now();
        float dt = std::chrono::duration<float>(now - lastTime).count();
        lastTime = now;

        frameTimes.push_back(dt);
        if (frameTimes.size() > sampleWindow)
            frameTimes.pop_front();

        // Calculate standard deviation of delta times (frame jitter)
        float mean = 0.0f;
        for (float t : frameTimes) mean += t;
        mean /= static_cast<float>(frameTimes.size());

        float variance = 0.0f;
        for (float t : frameTimes) variance += (t - mean) * (t - mean);
        variance /= static_cast<float>(frameTimes.size());

        entropyRate = std::sqrt(variance);

        if (gAIBackendPtr)
            gAIBackendPtr->Log("EntropyPredictor :: EntropyRate = " + std::to_string(entropyRate));
    }

}
