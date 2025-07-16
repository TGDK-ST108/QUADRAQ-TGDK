#ifndef TGDK_IAIBACKEND_HPP
#define TGDK_IAIBACKEND_HPP

#include <string>
#include <memory>
#include <iostream>

// ====================================================================
//                       Abstract AI Backend
// ====================================================================
class IAIBackend {
public:
    std::unique_ptr<IAIBackend> CreateOliviaBridgeIfEnabled();
    virtual bool Initialize() = 0;
    virtual void OnFrame() = 0;
    virtual void Log(const std::string& msg) = 0;
    virtual void LogError(const std::string& msg) = 0;
    virtual bool IsOliviaActive() const = 0;
    virtual bool ShouldSuppressDraw(float entropy) = 0;
    virtual std::string Identify() const = 0;
    virtual std::string GetStatusString() const = 0;
    virtual std::string GetBackendName() const { return "IAIBackend"; }
    virtual void OnFrameStart() {}
    virtual void OnFrameEnd() {}
    virtual void OnInterceptEvent(const std::string&) {}
    virtual void Shutdown() {}
    virtual std::string Query(const std::string& input) {
        return "[IAIBackend] Default query handler.";
    }

    virtual ~IAIBackend() = default;
};

// ====================================================================
//                       Global Backend Access
// ====================================================================

// Owned backend instance
IAIBackend* GetAIBackend();
void SetAIBackend(std::unique_ptr<IAIBackend> backend);
void ClearAIBackend();

// Raw pointer compatibility (for legacy modules)
extern IAIBackend* gAIBackendPtr;

#endif // TGDK_IAIBACKEND_HPP
