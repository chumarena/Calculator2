#pragma once

#include "plugin.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

#ifdef _WIN32
#include <windows.h>
using PluginHandle = HMODULE;
#else 
using PluginHandle = void*;
#endif

class PluginLoader {
public:
    PluginLoader();
    ~PluginLoader();

    void loadPlugins(const std::string& pluginDir = "plugins");

    const std::map<std::string, std::unique_ptr<IPlugin>>& getAvailablePlugins() const;

private:
    std::map<std::string, std::unique_ptr<IPlugin>> plugins;
    std::vector<PluginHandle> handles;

    
    PluginHandle loadLibrary(const std::string& path);
    void* getFunction(PluginHandle handle, const std::string& funcName);
    void freeLibrary(PluginHandle handle);
};