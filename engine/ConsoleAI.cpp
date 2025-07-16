/* ====================================================================
//                        ConsoleAI.cpp
//      Simple Implementation for Fallback AI Backend
// ====================================================================*/

#include "ConsoleAI.hpp"

bool ConsoleAI::Initialize() {
    std::cout << "[ConsoleAI] Initialized successfully.\n";
    return true;
}

void ConsoleAI::OnFrame() {
    std::cout << "[ConsoleAI] Frame tick.\n";
}

void ConsoleAI::Log(const std::string& msg) {
    std::cout << "[ConsoleAI] Log: " << msg << std::endl;
}

void ConsoleAI::LogError(const std::string& msg) {
    std::cerr << "[ConsoleAI] Error: " << msg << std::endl;
}

bool ConsoleAI::IsOliviaActive() const {
    return false;  // ConsoleAI always returns false for Olivia mode
}

bool ConsoleAI::ShouldSuppressDraw(float entropy) {
    return entropy < 0.25f; // arbitrary threshold for suppression
}
