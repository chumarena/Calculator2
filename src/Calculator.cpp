#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <locale> 




#include "PluginLoader.h" 
#include "Parser.h"      
#include "ExpressionTree.h" 

using namespace std;


void print_initial_status(const PluginLoader& loader) {
    const auto& availablePlugins = loader.getAvailablePlugins();
    
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
}


int main() {
    
 
    
    setlocale(LC_ALL, "ru");
    std::cout.imbue(std::locale(""));
    std::cerr.imbue(std::locale(""));
    
    PluginLoader loader;

    
    try {
        loader.loadPlugins("plugins"); 
    }
    catch (const std::exception& e) {
        std::cerr << "ОШИБКА ЗАГРУЗКИ ПЛАГИНОВ: " << e.what() << "\n";
    }
    
    print_initial_status(loader);

    std::string input;
    while (true) {
        
        
        try {
            
            loader.loadPlugins("plugins"); 
        }
        catch (const std::exception& e) {
            std::cerr << "Предупреждение: Проблема при обновлении плагинов: " << e.what() << "\n";
        }
        
        
        const auto& currentPlugins = loader.getAvailablePlugins();
        Parser parser(currentPlugins); 
        
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

            ExpressionTree tree(currentPlugins); 
            tree.build(rpn_tokens);
            double result = tree.evaluate();

            std::cout << "Результат: " << result << "\n";

        }
        catch (const std::exception& e) {
            std::cerr << "Ошибка: " << e.what() << "\n";
        }
        catch (...) {
            std::cerr << "Неизвестная ошибка.\n";
        }
    }

    return 0;
}