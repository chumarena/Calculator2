#pragma once

#include <string>
#include <memory> 
#include <vector>
#include <map>
#include "plugin.h"


class Node {
public:
    std::string value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;

    Node(const std::string& val) : value(val), left(nullptr), right(nullptr) {}
    Node(const std::string& op, std::unique_ptr<Node> l, std::unique_ptr<Node> r)
        : value(op), left(std::move(l)), right(std::move(r)) {
    }

    ~Node() = default;
};


class ExpressionTree {
public:
    ExpressionTree(const std::map<std::string, std::unique_ptr<IPlugin>>& availablePlugins);

    void build(const std::vector<std::string>& rpn_tokens);
    double evaluate() const;

private:
    std::unique_ptr<Node> root = nullptr;
    const std::map<std::string, std::unique_ptr<IPlugin>>& plugins;

    double evaluate_recursive(const Node* node) const;
    bool is_operator_or_function(const std::string& token) const;
};