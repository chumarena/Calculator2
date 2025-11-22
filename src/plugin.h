#pragma once

#include <string>
#include <memory>

enum class OpType {
    UNARY,  
    BINARY  
};

class IPlugin {
public:
    virtual ~IPlugin() = default;

    virtual std::string getName() const = 0;

    virtual OpType getType() const = 0;

    virtual double calculate(double a, double b = 0.0) = 0;
};


extern "C" __declspec(dllexport) IPlugin* createPlugin();