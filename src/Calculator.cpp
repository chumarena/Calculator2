#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include "PluginLoader.h" 
#include "Parser.h"      
#include "ExpressionTree.h" 

int main() {
	setlocale(LC_ALL, "ru");
    PluginLoader loader;

    try {
        // Загрузка DLL из папки "plugins"
        loader.loadPlugins("plugins");
    }
    catch (const std::exception& e) {
        std::cerr << "ОШИБКА ЗАГРУЗКИ ПЛАГИНОВ: " << e.what() << "\n";
    }

    const auto& availablePlugins = loader.getAvailablePlugins();
    Parser parser(availablePlugins);

    std::cout << "Калькулятор\n";
    std::cout << "Базовые операции: +, -, *, /\n";
    std::cout << "Загруженные плагины: ";
    if (availablePlugins.empty()) {
        std::cout << "Нет\n";
    }
    else {
        for (const auto& pair : availablePlugins) {
            std::cout << pair.first << "(" << (pair.second->getType() == OpType::UNARY ? "унарная" : "бинарная") << ") ";
        }
        std::cout << "\n";
    }
    std::cout << "Введите 'exit' или 'quit' для завершения.\n";

    std::string input;
    while (true) {
        std::cout << "\nВведите выражение: ";
        std::getline(std::cin, input);

        std::string lower_input = input;
        std::transform(lower_input.begin(), lower_input.end(), lower_input.begin(),
            [](unsigned char c) { return std::tolower(c); });

        if (lower_input == "exit" || lower_input == "quit" || std::cin.eof()) {
            std::cout << "Завершение работы калькулятора.\n";
            break;
        }

        if (input.empty()) continue;

        try {
            std::vector<std::string> tokens = parser.tokenize(input);
            std::vector<std::string> rpn_tokens = parser.infix_to_rpn(tokens);

            ExpressionTree tree(availablePlugins);
            tree.build(rpn_tokens);

            double result = tree.evaluate();

            std::cout << "= " << result << std::endl;

        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << std::endl;
        }
    }

    return 0;
}