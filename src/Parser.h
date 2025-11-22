#pragma once

#include <string>
#include <vector>
#include <map>
#include <memory>
#include "plugin.h"

class Parser {
public:
    Parser(const std::map<std::string, std::unique_ptr<IPlugin>>& availablePlugins);

    std::vector<std::string> tokenize(const std::string& expression);
    std::vector<std::string> infix_to_rpn(const std::vector<std::string>& tokens);

private:
    const std::map<std::string, std::unique_ptr<IPlugin>>& plugins;

    int get_precedence(const std::string& op);
    bool is_operator(const std::string& token);
    bool is_function(const std::string& token);
    static bool is_number(const std::string& token);
};
