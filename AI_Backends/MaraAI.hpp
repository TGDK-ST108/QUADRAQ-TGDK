#ifndef TGDK_MARA_AI_HPP
#define TGDK_MARA_AI_HPP

#include "TGDK_IAIBackend.hpp"
#include <string>

class MaraAI : public IAIBackend {
public:
    void Log(const std::string& msg) override;
    void LogError(const std::string& msg) override;
    std::string Identify() override;
    std::string Query(const std::string& input) override;
    void OnFrameStart() override;
    void OnFrameEnd() override;
    void OnInterceptEvent(const std::string& event) override;
    void Shutdown() override;
};

extern "C" __declspec(dllexport) IAIBackend* CreateAIBackend();

#endif
