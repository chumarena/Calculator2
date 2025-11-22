#include "plugin.h"
#include <cmath>

class PowPlugin : public IPlugin {
public:
    std::string getName() const override { return "^"; } 
    OpType getType() const override { return OpType::BINARY; }
    
    double calculate(double a, double b) override {
        return std::pow(a, b); 
    }
};

extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new PowPlugin();
}