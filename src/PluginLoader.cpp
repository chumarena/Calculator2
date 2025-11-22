#include "PluginLoader.h"
#include <iostream>
#include <filesystem>
#include <stdexcept>


namespace fs = std::filesystem;
using namespace std;



PluginHandle PluginLoader::loadLibrary(const string& path) {
    return LoadLibraryA(path.c_str());
}
void* PluginLoader::getFunction(PluginHandle handle, const string& funcName) {
    return (void*)GetProcAddress(handle, funcName.c_str());
}
void PluginLoader::freeLibrary(PluginHandle handle) {
    if (handle) FreeLibrary(handle);
}


PluginLoader::PluginLoader() {}

PluginLoader::~PluginLoader() {
    
    plugins.clear();
    
    for (PluginHandle handle : handles) {
        freeLibrary(handle);
    }
}

void PluginLoader::loadPlugins(const string& pluginDir) {
    if (!fs::exists(pluginDir) || !fs::is_directory(pluginDir)) {
        
        throw runtime_error("Папка плагинов не найдена: " + pluginDir);
    }

    plugins.clear();
    for (PluginHandle handle : handles) freeLibrary(handle);
    handles.clear();

    for (const auto& entry : fs::directory_iterator(pluginDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".dll") {
            string path = entry.path().string();
            PluginHandle handle = loadLibrary(path);

            if (handle) {
                handles.push_back(handle);
                using CreatePluginFunc = IPlugin * (*)();
                CreatePluginFunc creator = (CreatePluginFunc)getFunction(handle, "createPlugin");

                if (creator) {
                    IPlugin* pluginPtr = creator();
                    if (pluginPtr) {
                        string name = pluginPtr->getName();
                        if (plugins.count(name)) {
                            
                            cerr << "Плагин '" << name << "' уже загружен. Пропуск." << endl;
                            delete pluginPtr;
                        }
                        else {
                            plugins[name] = unique_ptr<IPlugin>(pluginPtr);
                            
                            cout << "Загружен плагин: " << name << endl;
                        }
                    }
                    else {
                        
                        cerr << "Ошибка: createPlugin вернул nullptr для " << path << endl;
                    }
                }
                else {
                    
                    cerr << "Ошибка: Не найдена функция 'createPlugin' в " << path << endl;
                }
            }
            else {
                
                cerr << "Ошибка: Не удалось загрузить библиотеку " << path << endl;
            }
        }
    }
}

const map<string, unique_ptr<IPlugin>>& PluginLoader::getAvailablePlugins() const {
    return plugins;
}