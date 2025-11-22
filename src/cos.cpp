#include "plugin.h" 
#include <cmath>

class CosPlugin : public IPlugin {
public:
    std::string getName() const override { return "cos"; }
    OpType getType() const override { return OpType::UNARY; }
    
    double calculate(double a, double b = 0.0) override {
        return std::cos(a); 
    }
};


extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new CosPlugin();
}