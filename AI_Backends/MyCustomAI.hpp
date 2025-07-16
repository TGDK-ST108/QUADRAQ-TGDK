#pragma once

#include "TGDK_IAIBackend.hpp"  // or "IAIBackend.hpp" if that's the file
#include <vector>

namespace QUADRAQ {

    class MyCustomAI : public IAIBackend {
    public:
        MyCustomAI();

        void initialize();
        void run();
        void setInput(const std::vector<float>& input);
        std::vector<float> getOutput() const;
        void OnFrame() override;

    private:
        std::vector<float> inputData;
        std::vector<float> outputData;

        void compute();
    };

} // namespace QUADRAQ
