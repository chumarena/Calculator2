#include "plugin.h" 
#include <cmath>
#include <stdexcept>

class LnPlugin : public IPlugin {
public:
    std::string getName() const override { return "ln"; }
    OpType getType() const override { return OpType::UNARY; }
    

    double calculate(double a, double b = 0.0) override {
        if (a <= 0.0) {
            throw std::runtime_error("Ошибка: Натуральный логарифм (ln) от неположительного числа.");
        }
        return std::log(a); 
    }
};


extern "C" __declspec(dllexport) IPlugin* createPlugin() {
    return new LnPlugin();

}
