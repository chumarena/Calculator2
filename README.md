# Calculator2

Для корректного запуска exe файла с использхованием dll библиотек, папка plugins и exe должны находиться на одном уровне

Инструкция по сборке  и запуска программы через CMake(Требуется находиться в папке проекта)
mkdir build
cd build

cmake ..

cmake --build . --config Release

cd .\bin\Release\
.\calc.exe
