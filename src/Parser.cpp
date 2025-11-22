#include "Parser.h"
#include <stack>
#include <stdexcept>
#include <cctype>
#include <algorithm>
#include <sstream>

using namespace std;

bool Parser::is_number(const string& token) {
    stringstream ss(token);
    double d;
    return (ss >> d) && ss.eof() && !ss.fail();
}

Parser::Parser(const map<string, unique_ptr<IPlugin>>& availablePlugins)
    : plugins(availablePlugins) {
}

bool Parser::is_function(const string& token) {
    auto it = plugins.find(token);
    return it != plugins.end() && it->second->getType() == OpType::UNARY;
}

bool Parser::is_operator(const string& token) {
    // 1. Стандартные бинарные операторы (+, -, *, /)
    if (token.length() == 1 && token.find_first_of("+-*/") != string::npos) {
        return true;
    }
    // 2. Бинарные операторы из плагинов (e.g., ^)
    auto it = plugins.find(token);
    if (it != plugins.end() && it->second->getType() == OpType::BINARY) {
        return true;
    }
    return false;
}

int Parser::get_precedence(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;

    auto it = plugins.find(op);
    if (it != plugins.end() && it->second->getType() == OpType::BINARY) {
        return 3; //Самый высокий приоритет для бинарных плагинов (как у ^)
    }
    return 0;
}

vector<string> Parser::tokenize(const string& expression) {
    vector<string> tokens;
    for (int i = 0; i < expression.length(); ++i) {
        char c = expression[i];
        if (isspace(c)) continue;

        if (isdigit(c) || c == '.') {
            string num;
            while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
                num += expression[i++];
            }
            --i;
            tokens.push_back(num);
        }
        else if (isalpha(c)) {
            string func;
            while (i < expression.length() && isalpha(expression[i])) {
                func += expression[i++];
            }
            --i;
            transform(func.begin(), func.end(), func.begin(), ::tolower);
            if (is_function(func) || is_operator(func)) {
                tokens.push_back(func);
            }
            else {
                throw runtime_error("Неизвестная функция или идентификатор: " + func);
            }
        }
        else {
            string token(1, c);
            if (token == "(" || token == ")") {
                tokens.push_back(token);
            }
            else if (is_operator(token) || is_function(token)) {
                tokens.push_back(token);
            }
            else {
                throw runtime_error("Недопустимый символ в выражении: " + token);
            }
        }
    }
    return tokens;
}

vector<string> Parser::infix_to_rpn(const vector<string>& tokens) {
    vector<string> rpn_output;
    stack<string> op_stack;
    const string UNARY_MINUS = "~";

    for (int i = 0; i < tokens.size(); ++i) {
        const string& token = tokens[i];

        bool is_potential_unary = (token == "-");

        if (is_potential_unary) {
            bool is_start = (i == 0);
            bool is_after_paren = (i > 0 && tokens[i - 1] == "(");

           
            bool is_after_operator = false;
            if (i > 0) {
                const string& prev_token = tokens[i - 1];

                
                if (is_operator(prev_token)) {
                    is_after_operator = true;
                }
                
                else if (is_function(prev_token)) {
                    is_after_operator = true;
                }
            }

            
            if (is_start || is_after_paren || is_after_operator) {
                op_stack.push(UNARY_MINUS);
                continue;
            }
        }

        if (is_number(token)) {
            rpn_output.push_back(token);
        }
        else if (is_function(token)) {
            op_stack.push(token);
        }
        else if (is_operator(token)) { 
            while (!op_stack.empty()) {
                const string& top = op_stack.top();

                // Унарный минус имеет самый высокий приоритет (как функция)
                int top_precedence = (top == UNARY_MINUS) ? 4 : get_precedence(top);
                int token_precedence = get_precedence(token);

                if (is_function(top) || top == UNARY_MINUS ||
                    (is_operator(top) &&
                        (top_precedence > token_precedence ||
                            (top_precedence == token_precedence &&
                                token_precedence < 3)))) { //Левоассоциативность для +, -, *, /

                    rpn_output.push_back(top);
                    op_stack.pop();
                }
                else {
                    break;
                }
            }
            op_stack.push(token);
        }
        else if (token == "(") {
            op_stack.push(token);
        }
        else if (token == ")") {
            while (!op_stack.empty() && op_stack.top() != "(") {
                rpn_output.push_back(op_stack.top());
                op_stack.pop();
            }
            if (op_stack.empty()) throw runtime_error("Несогласованные скобки.");
            op_stack.pop();

            if (!op_stack.empty()) {
                const string& top = op_stack.top();
                if (is_function(top) || top == UNARY_MINUS) {
                    rpn_output.push_back(top);
                    op_stack.pop();
                }
            }
        }
        else {
            throw runtime_error("Неизвестный токен: " + token);
        }
    }

    while (!op_stack.empty()) {
        if (op_stack.top() == "(" || op_stack.top() == ")") {
            throw runtime_error("Несогласованные скобки.");
        }
        rpn_output.push_back(op_stack.top());
        op_stack.pop();
    }

    return rpn_output;
}
