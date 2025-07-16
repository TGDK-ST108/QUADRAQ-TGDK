#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "TGDK_IAIBackend.hpp"

// No need to include AIRegistry.hpp inside itself — that line was redundant and is now removed.

class AIRegistry {
public:
    static bool RegisterAI(const std::string& label, const std::string& dllPath);
    static IAIBackend* Get(const std::string& label);
    static void PrintRegistered();
    static void Clear();

    // ? Correct: Single, clean declaration of Register
    static void Register(const std::string& id, std::unique_ptr<IAIBackend> backend);

private:
    // ? Hidden map — properly isolated
    static std::unordered_map<std::string, std::unique_ptr<IAIBackend>>& Registry();
};
