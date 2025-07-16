#ifndef TGDK_MARA_AI_HPP
#define TGDK_MARA_AI_HPP

#include "TGDK_IAIBackend.hpp"
#include <string>
#include <iostream>

class MaraAI : public IAIBackend {
public:
    // === Core IAIBackend Interface ===
    std::string Query(const std::string& input) override;
    void OnFrameStart() override;
    void OnFrameEnd() override;
    void OnInterceptEvent(const std::string& event) override;
    void Shutdown() override;

    // === QUADRAQ Extended Interface ===
    bool Initialize() override;
    void OnFrame() override;
    void Log(const std::string& msg) override;
    void LogError(const std::string& msg) override;
    bool IsOliviaActive() const override;
    bool ShouldSuppressDraw(float entropy) override;
    std::string Identify() const override;
    std::string GetStatusString() const override;
};

// Exported creator for dynamic linkage
extern "C" __declspec(dllexport) IAIBackend* CreateAIBackend();

#endif // TGDK_MARA_AI_HPP
