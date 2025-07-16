#ifndef OLIVIA_AI_HPP
#define OLIVIA_AI_HPP

#include "TGDK_IAIBackend.hpp"
#include <string>

class OliviaAI : public IAIBackend {
public:
    // Inherited overrides
    std::string Query(const std::string& input) override;
    void OnFrameStart() override;
    void OnFrameEnd() override;
    void OnInterceptEvent(const std::string& event) override;
    void Shutdown() override;

    // Fully implemented or concrete overrides
    bool Initialize() override;
    void OnFrame() override;
    void Log(const std::string& msg) override;
    void LogError(const std::string& msg) override;
    bool IsOliviaActive() const override;
    bool ShouldSuppressDraw(float entropy) override;
    std::string Identify() const override;
    std::string GetStatusString() const override;
    std::string GetBackendName() const override { return "OliviaAI"; }

    ~OliviaAI() override;
};

extern "C" __declspec(dllexport) IAIBackend* CreateAIBackend();

#endif // OLIVIA_AI_HPP
