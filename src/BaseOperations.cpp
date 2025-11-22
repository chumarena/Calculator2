#include "BaseOperations.h"
#include <stdexcept>


double Add::calculate(double a, double b) const {
    return a + b;
}


double Subtract::calculate(double a, double b) const {
    return a - b;
}


double Multiply::calculate(double a, double b) const {
    return a * b;
}


double Divide::calculate(double a, double b) const {
    if (b == 0.0) {
        throw std::runtime_error("Деление на ноль невозможно.");
    }
    return a / b;
}