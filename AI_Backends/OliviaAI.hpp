
#ifndef OLIVIA_AI_HPP
#define OLIVIA_AI_HPP

#include <string>

namespace OliviaAI {
    void Initialize();
    void Pulse();
    void Log(const std::string& msg);
    void LogError(const std::string& msg);
}

#endif // OLIVIA_AI_HPP
