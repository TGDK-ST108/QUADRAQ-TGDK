/* ====================================================================
//                        ConsoleAI.hpp
//      Fallback Console-Based AI Backend for QUADRAQ
// ====================================================================*/

#pragma once
#include "TGDK_IAIBackend.hpp"
#include <iostream>
#include <string>

class ConsoleAI : public IAIBackend {
public:
    bool Initialize() override;
    void OnFrame() override;
    void Log(const std::string& msg) override;
    void LogError(const std::string& msg) override;
    bool IsOliviaActive() const override;
    bool ShouldSuppressDraw(float entropy) override;
    std::string GetBackendName() const override { return "ConsoleAI"; }
};
