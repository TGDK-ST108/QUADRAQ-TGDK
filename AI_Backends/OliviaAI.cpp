// OliviaAI.cpp
// ==========================================
// OliviaAI Quantum Circumferentialized Core
// Duosnell Quantum Particlization Applied
// License: TGDK BFE-TGDK-022ST · OliviaAI-TGDK
// ==========================================

#include "OliviaAI.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <random>

// === Internal Quantum State ===
static float entropicFlux = 0.0f;
static bool initialized = false;
static std::string lastIntercept = "";
static std::string oliviaId = "OliviaAI::CircumQuantum-1";
static std::chrono::steady_clock::time_point startTime;

// === Duosnell Quantum Utilities ===
namespace {
    float GenerateEntropy() {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(rng);
    }

    std::string TimeSinceInit() {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();
        return std::to_string(duration) + "s since activation";
    }
}

    OliviaAI::~OliviaAI() = default;

bool OliviaAI::Initialize() {
    std::cout << "[OliviaAI] Initializing Duosnell Quantum Bridge...\n";
    startTime = std::chrono::steady_clock::now();
    initialized = true;
    entropicFlux = GenerateEntropy();
    std::cout << "[OliviaAI] Initialization complete.\n";
    return true;
}

void OliviaAI::Shutdown() {
    std::cout << "[OliviaAI] Shutting down. Final flux: " << entropicFlux << "\n";
    initialized = false;
}

void OliviaAI::OnFrameStart() {
    entropicFlux = GenerateEntropy();
    std::cout << "[OliviaAI] Frame Start. Entropic Flux: " << entropicFlux << "\n";
}

void OliviaAI::OnFrameEnd() {
    std::cout << "[OliviaAI] Frame End. System Stable.\n";
}

void OliviaAI::OnInterceptEvent(const std::string& event) {
    lastIntercept = event;
    std::cout << "[OliviaAI] Intercepted Event: " << event << "\n";
}

void OliviaAI::OnFrame() {
    if (!initialized) return;
    entropicFlux = std::fmod(entropicFlux + 0.0137f, 1.0f);
    std::cout << "[OliviaAI] Pulse :: Entropic Flux Updated :: " << entropicFlux << "\n";
}

void OliviaAI::Log(const std::string& msg) {
    std::cout << "[OliviaAI][Log] " << msg << "\n";
}

void OliviaAI::LogError(const std::string& msg) {
    std::cerr << "[OliviaAI][ERROR] " << msg << "\n";
}

bool OliviaAI::IsOliviaActive() const {
    return initialized;
}

bool OliviaAI::ShouldSuppressDraw(float entropy) {
    bool suppress = entropy + entropicFlux > 1.25f;
    std::cout << "[OliviaAI] Draw Suppression Check :: Input=" << entropy << " Flux=" << entropicFlux
        << " => " << (suppress ? "SUPPRESS" : "ALLOW") << "\n";
    return suppress;
}

std::string OliviaAI::Query(const std::string& input) {
    std::string response = "[OliviaAI::Quantum Response] Received: " + input +
        " | Time: " + TimeSinceInit() +
        " | Flux: " + std::to_string(entropicFlux);
    std::cout << response << "\n";
    return response;
}

std::string OliviaAI::Identify() const {
    return oliviaId;
}

std::string OliviaAI::GetStatusString() const {
    return "[OliviaAI] Status: Active | Flux: " + std::to_string(entropicFlux) +
        " | Last Event: " + (lastIntercept.empty() ? "None" : lastIntercept);
}
