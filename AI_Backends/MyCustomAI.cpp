// MyCustomAI.cpp
// TGDK QUADRAQ AI Backend - Custom User Implementation

#include "MyCustomAI.hpp"
#include <iostream>

namespace QUADRAQ {

    MyCustomAI::MyCustomAI() {
        std::cout << "[MyCustomAI] Constructor called.\n";
    }

    void MyCustomAI::initialize() {
        std::cout << "[MyCustomAI] Initialization complete.\n";
        // Load models or perform setup here
    }

    void MyCustomAI::run() {
        std::cout << "[MyCustomAI] Running inference...\n";
        compute();
    }

    void MyCustomAI::setInput(const std::vector<float>& input) {
        inputData = input;
        std::cout << "[MyCustomAI] Input data set (" << inputData.size() << " values).\n";
    }

    std::vector<float> MyCustomAI::getOutput() const {
        return outputData;
    }

    void MyCustomAI::compute() {
        // Example: dummy transform of input ? output
        outputData.clear();
        for (float val : inputData) {
            outputData.push_back(val * 2.0f); // simple dummy logic
        }
        std::cout << "[MyCustomAI] Computation complete. Output size: " << outputData.size() << "\n";
    }

} // namespace QUADRAQ
