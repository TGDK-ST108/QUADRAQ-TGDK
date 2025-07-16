
// ====================================================================
//                        OliviaAI.cpp
//     TGDK Closed Bridge AI Interface for QUADRAQ + TGDK Systems
//     License: TGDK BFE-TGDK-022ST • Sean Tichenor • All Rights Reserved
// ====================================================================

#include "OliviaAI.hpp"
#include <iostream>
#include <mutex>
#include <fstream>
#include <ctime>

static std::mutex olivia_mutex;
static bool initialized = false;

namespace OliviaAI {

    void Initialize() {
        std::lock_guard<std::mutex> lock(olivia_mutex);
        if (initialized) return;

        Log("OliviaAI :: Initializing AI Context...");
        // Placeholder for real AI backend init logic
        initialized = true;
    }

    void Pulse() {
        std::lock_guard<std::mutex> lock(olivia_mutex);
        if (!initialized) return;

        // This could connect to Olivia's logic cycle
        Log("OliviaAI :: Pulse()");
    }

    void Log(const std::string& msg) {
        std::lock_guard<std::mutex> lock(olivia_mutex);
        std::cout << "[OliviaAI] " << msg << std::endl;
        AppendToLogFile("[INFO] " + msg);
    }

    void LogError(const std::string& msg) {
        std::lock_guard<std::mutex> lock(olivia_mutex);
        std::cerr << "[OliviaAI:ERROR] " << msg << std::endl;
        AppendToLogFile("[ERROR] " + msg);
    }

    void AppendToLogFile(const std::string& msg) {
        std::ofstream logFile("OliviaAI.log", std::ios::app);
        if (!logFile.is_open()) return;

        std::time_t now = std::time(nullptr);
        char* dt = std::ctime(&now);
        if (dt) dt[strlen(dt) - 1] = '\0'; // remove newline

        logFile << "[" << dt << "] " << msg << "\n";
    }

}
