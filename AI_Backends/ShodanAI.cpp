// ShodanAI.cpp
// ==========================================
// ShodanAI Quantum Surveillance Node Core
// Duosnell Quantum Particlization Applied
// License: TGDK BFE-TGDK-022ST · OliviaAI-TGDK
// ==========================================

#include "ShodanAI.hpp"
#include <iostream>
#include <chrono>
#include <thread>
#include <cmath>
#include <random>

// === Internal Surveillance State ===
static float threatIndex = 0.0f;
static bool surveillanceOnline = false;
static std::string lastPing = "";
static std::string shodanId = "ShodanAI::SentinelNode-9";
static std::chrono::steady_clock::time_point bootTime;

// === Duosnell Quantum Utility ===
namespace {
    float GenerateThreatIndex() {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(rng);
    }

    std::string TimeSinceBoot() {
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - bootTime).count();
        return std::to_string(duration) + "s operational";
    }
}

ShodanAI::~ShodanAI() = default;

bool ShodanAI::Initialize() {
    std::cout << "[ShodanAI] Activating Sentinel Node...\n";
    bootTime = std::chrono::steady_clock::now();
    surveillanceOnline = true;
    threatIndex = GenerateThreatIndex();
    std::cout << "[ShodanAI] Node active. Initial threat index: " << threatIndex << "\n";
    return true;
}

void ShodanAI::Shutdown() {
    std::cout << "[ShodanAI] Shutdown sequence initiated. Final threat index: " << threatIndex << "\n";
    surveillanceOnline = false;
}

void ShodanAI::OnFrameStart() {
    // No-op placeholder if needed
}

void ShodanAI::OnFrameEnd() {
    std::cout << "[ShodanAI] Frame End :: Threat Index = " << threatIndex << "\n";
}

void ShodanAI::OnInterceptEvent(const std::string& event) {
    lastPing = event;
    std::cout << "[ShodanAI] Intercepted Comm :: " << event << "\n";
    threatIndex = std::min(1.0f, threatIndex + 0.05f); // escalate threat slightly
}

void ShodanAI::OnFrame() {
    if (!surveillanceOnline) return;
    threatIndex *= 0.975f;
    std::cout << "[ShodanAI] Frame Tick :: Threat Index decay -> " << threatIndex << "\n";
}

void ShodanAI::Log(const std::string& msg) {
    std::cout << "[ShodanAI][Log] " << msg << "\n";
}

void ShodanAI::LogError(const std::string& msg) {
    std::cerr << "[ShodanAI][ERROR] " << msg << "\n";
}

bool ShodanAI::IsOliviaActive() const {
    return surveillanceOnline;
}

bool ShodanAI::ShouldSuppressDraw(float entropy) {
    bool suppress = (entropy + threatIndex) > 1.1f;
    std::cout << "[ShodanAI] Draw Decision: " << (suppress ? "SUPPRESS" : "RENDER")
        << " | Entropy: " << entropy << " | Threat Index: " << threatIndex << "\n";
    return suppress;
}

std::string ShodanAI::Identify() const {
    return shodanId;
}

std::string ShodanAI::GetStatusString() const {
    return "[ShodanAI] Status: " + std::string(surveillanceOnline ? "Online" : "Offline") +
        " | Threat Index: " + std::to_string(threatIndex) +
        " | Last Ping: " + (lastPing.empty() ? "None" : lastPing);
}
std::string ShodanAI::Query(const std::string& input) {
    return "[ShodanAI::Query] Response to: " + input;
}
