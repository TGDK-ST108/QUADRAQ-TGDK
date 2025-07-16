/* ====================================================================
//                    TGDK_IAIBackend.hpp
//    Interface for AI Logging / Control (OliviaAI-compatible)
//    License: TGDK BFE-TGDK-022ST • Sean Tichenor • All Rights Reserved
// ====================================================================*/

#ifndef TGDK_IAIBACKEND_HPP
#define TGDK_IAIBACKEND_HPP

#include <string>

class IAIBackend {
public:
    // Basic system hooks
    virtual bool Initialize() = 0;
    virtual void OnFrame() = 0;

    // Logging output
    virtual void Log(const std::string& msg) = 0;
    virtual void LogError(const std::string& msg) = 0;

    // Required control queries
    virtual bool IsOliviaActive() const = 0;
    virtual bool ShouldSuppressDraw(float entropy) = 0;
    virtual std::string Identify() const = 0;
    virtual std::string GetStatusString() const = 0;

    // Optional metadata for identification
    virtual std::string GetBackendName() const { return "IAIBackend"; }

    virtual ~IAIBackend() = default;
};

// Switch global AI backend (true = OliviaAI, false = fallback like ConsoleAI)
void SwitchAIBackend(bool useOlivia);

// Return true if current AI is Olivia
bool IsOliviaActive();

// Globally exposed backend pointer (owned by internal unique_ptr)
extern IAIBackend* gAIBackendPtr;

#endif // TGDK_IAIBACKEND_HPP
