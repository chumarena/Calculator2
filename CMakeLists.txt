cmake_minimum_required(VERSION 3.15)
project(CalculatorApp CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    
    foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()


include_directories("${PROJECT_SOURCE_DIR}/src")


set(OUTPUT_DIR "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${OUTPUT_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${OUTPUT_DIR})


set(CORE_SOURCES 
    src/Calculator.cpp
    src/Parser.cpp
    src/PluginLoader.cpp
    src/ExpressionTree.cpp
    src/BaseOperations.cpp
)


add_executable(calc ${CORE_SOURCES})


if(WIN32)
    target_link_libraries(calc PRIVATE kernel32 user32)
endif()



macro(add_calculator_plugin plugin_name source_file)
    add_library(${plugin_name}_lib SHARED ${source_file})

    set_target_properties(${plugin_name}_lib PROPERTIES 
        OUTPUT_NAME ${plugin_name}
        PREFIX ""
        RUNTIME_OUTPUT_DIRECTORY "$<TARGET_FILE_DIR:calc>/plugins"
    )
endmacro()


add_calculator_plugin(sin src/sin.cpp)
add_calculator_plugin(cos src/cos.cpp)
add_calculator_plugin(ln src/ln.cpp)
add_calculator_plugin(pow src/pow.cpp)