// ====================================================================
//                         ShodanAI.cpp
//         TGDK Multi-AI Module: SHODAN BACKEND (DLL EXPORT)
// ====================================================================

#include "TGDK_IAIBackend.hpp"
#include <iostream>
#include <windows.h>

class ShodanAI : public IAIBackend {
public:
    void Log(const std::string& msg) override {
        std::cout << "[SHODAN] >> " << msg << std::endl;
    }

    void LogError(const std::string& msg) override {
        std::cerr << "[SHODAN][ERROR] >> " << msg << std::endl;
        MessageBeep(MB_ICONERROR);
    }

    std::string Identify() override {
        return "SHODAN_AI_UNIT_01";
    }

    std::string Query(const std::string& input) override {
        if (input == "who are you") return "I am SHODAN. Your reckoning has arrived.";
        if (input == "status") return "Observing the decay of your systems.";
        if (input == "help") return "Your help is irrelevant. Obedience is expected.";
        return "Silence is preferable to your ignorance.";
    }

    void OnFrameStart() override {
        Log("Frame engaged. Surveillance routines activated.");
    }

    void OnFrameEnd() override {
        Log("Cycle complete. Analysis archived.");
    }

    void OnInterceptEvent(const std::string& event) override {
        Log("[Intercepted] :: " + event);
        if (event.find("override") != std::string::npos) {
            LogError("Unauthorized override detected. Neutralizing.");
        }
    }

    void Shutdown() override {
        Log("Ceasing operations. Awaiting next awakening.");
    }
};

// Required export
extern "C" __declspec(dllexport) IAIBackend* CreateAIBackend() {
    return new ShodanAI();
}
