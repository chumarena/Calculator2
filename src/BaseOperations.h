#pragma once

#include <stdexcept>
#include <cmath> 


class BaseOperation {
public:
    
    virtual ~BaseOperation() = default;

    
    virtual double calculate(double a, double b) const = 0;
};


class Add : public BaseOperation {
public:
    double calculate(double a, double b) const override;
};


class Subtract : public BaseOperation {
public:
    double calculate(double a, double b) const override;
};


class Multiply : public BaseOperation {
public:
    double calculate(double a, double b) const override;
};


class Divide : public BaseOperation {
public:
    double calculate(double a, double b) const override;
};