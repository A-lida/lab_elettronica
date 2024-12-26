cmake_minimum_required(VERSION 3.16)
project(Fit VERSION 0.1.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# richiedi l'uso di C++17, senza estensioni non-standard offerte dal compilatore usato 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# abilita warning
string(APPEND CMAKE_CXX_FLAGS " -Wall -Wextra")

find_package(ROOT COMPONENTS CONFIG REQUIRED)

add_executable(fit_caratteristica
        fit_caratteristica.cpp
        graphic_stuff.cpp
        data_handling.cpp)

target_include_directories(fit_caratteristica PRIVATE include)
target_link_libraries(fit_caratteristica PRIVATE  ROOT::Core ROOT::Hist ROOT::Physics ROOT::Gpad ROOT::ROOTDataFrame)

add_executable(grafico_aggregato
        grafico_aggregato.cpp
        graphic_stuff.cpp
        data_handling.cpp)

target_include_directories(grafico_aggregato PRIVATE include)
target_link_libraries(grafico_aggregato PRIVATE  ROOT::Core ROOT::Hist ROOT::Physics ROOT::Gpad ROOT::ROOTDataFrame)

add_executable(calc_params
        calc_params.cpp
        data_handling.cpp)

target_include_directories(calc_params PRIVATE include)
target_link_libraries(calc_params PRIVATE  ROOT::Core ROOT::Hist ROOT::Gpad ROOT::ROOTDataFrame)