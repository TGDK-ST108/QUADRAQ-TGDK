#ifndef TGDK_SHODAN_AI_HPP
#define TGDK_SHODAN_AI_HPP

#include "TGDK_IAIBackend.hpp"
#include <string>

class ShodanAI : public IAIBackend {
public:
    std::string Query(const std::string& input) override;
    void OnFrameStart() override;
    void OnFrameEnd() override;
    void OnInterceptEvent(const std::string& event) override;
    void Shutdown() override;
    virtual bool Initialize() = 0;
    virtual void OnFrame() = 0;
    virtual void Log(const std::string& msg) = 0;
    virtual void LogError(const std::string& msg) = 0;
    virtual bool IsOliviaActive() const = 0;
    virtual bool ShouldSuppressDraw(float entropy) = 0;
    virtual std::string Identify() const = 0;
    virtual std::string GetStatusString() const = 0;

};

extern "C" __declspec(dllexport) IAIBackend* CreateAIBackend();

#endif // TGDK_SHODAN_AI_HPP
