#include "AIRegistry.hpp"
#include <iostream>

// Internal registry map (hidden inside Registry())
std::unordered_map<std::string, std::unique_ptr<IAIBackend>>& AIRegistry::Registry() {
    static std::unordered_map<std::string, std::unique_ptr<IAIBackend>> instance;
    return instance;
}

// Register a backend by ID
void AIRegistry::Register(const std::string& id, std::unique_ptr<IAIBackend> backend) {
    Registry()[id] = std::move(backend);
}

// Placeholder for future DLL-based backend loading
bool AIRegistry::RegisterAI(const std::string& label, const std::string& dllPath) {
    std::cerr << "[AIRegistry] Dynamic DLL load not implemented yet: " << dllPath << std::endl;
    return false;
}

// Retrieve backend by ID
IAIBackend* AIRegistry::Get(const std::string& label) {
    auto& reg = Registry();
    auto it = reg.find(label);
    return (it != reg.end()) ? it->second.get() : nullptr;
}

// Print all registered IDs
void AIRegistry::PrintRegistered() {
    std::cout << "[AIRegistry] Registered Backends:\n";
    for (const auto& [id, _] : Registry()) {
        std::cout << " - " << id << "\n";
    }
}

// Clear all backends
void AIRegistry::Clear() {
    Registry().clear();
}
