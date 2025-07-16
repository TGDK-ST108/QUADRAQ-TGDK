// ShodanAI.hpp
#pragma once

#include <string>
#include "TGDK_IAIBackend.hpp"  // or the correct path if it's different

class ShodanAI : public IAIBackend {
public:
    virtual ~ShodanAI();  // ? THIS is required
    virtual bool Initialize();
    virtual void Shutdown();
    virtual void OnFrameStart();
    virtual void OnFrameEnd();
    virtual void OnInterceptEvent(const std::string& event);
    virtual void OnFrame();
    virtual void Log(const std::string& msg);
    virtual void LogError(const std::string& msg);
    virtual bool IsOliviaActive() const;
    virtual bool ShouldSuppressDraw(float entropy);
    virtual std::string Identify() const;
    virtual std::string GetStatusString() const;
    virtual std::string Query(const std::string& input); // Declaration only'
    std::string GetBackendName() const override { return "ShodanAI"; }

};

// Provide the default implementation in the .cpp file, not inline in the class