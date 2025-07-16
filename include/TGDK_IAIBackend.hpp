#pragma once

#include <memory>
#include <string>

class IAIBackend {
public:
    virtual ~IAIBackend() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown() {}

    virtual void Log(const std::string& msg) = 0;
    virtual void LogError(const std::string& msg) = 0;

    virtual void OnFrameStart() {}
    virtual void OnFrame() = 0;
    virtual void OnFrameEnd() {}

    virtual void OnInterceptEvent(const std::string& event) {} // <-- Add this line

    virtual bool IsOliviaActive() const { return false; }
    virtual bool ShouldSuppressDraw(float delta) { return false; }

    virtual std::string GetBackendName() const = 0;
    virtual std::string Identify() const = 0;
    virtual std::string GetStatusString() const = 0;
    virtual std::string Query(const std::string& input) = 0;
};

// Declare global pointer
extern IAIBackend* gAIBackendPtr;

// Backend routing functions
IAIBackend* GetAIBackend();
void SetAIBackend(std::unique_ptr<IAIBackend> backend);
void ClearAIBackend();

// Olivia bridge support
std::unique_ptr<IAIBackend> CreateOliviaBridgeIfEnabled();
bool IsOliviaActive();
std::string GetOliviaStatusString();