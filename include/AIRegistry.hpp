#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "TGDK_IAIBackend.hpp"
#include "QUADRAQ.hpp"

namespace QUADRAQ {

    class AIRegistry {
    public:
        static bool RegisterAI(const std::string& label, const std::string& dllPath);
        static IAIBackend* Get(const std::string& label);
        static void PrintRegistered();
        static void Clear();

        // ? Registers a backend manually by ID
        static void Register(const std::string& id, std::unique_ptr<IAIBackend> backend);

    private:
        // ? Internal registry (hidden)
        static std::unordered_map<std::string, std::unique_ptr<IAIBackend>>& Registry();
    };

} // namespace QUADRAQ
