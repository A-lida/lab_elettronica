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

add_executable(fit_calibrazione fit_calibrazione.cpp) 
add_executable(fit_diodi fit_diodi.cpp) 

target_link_libraries(fit_calibrazione PRIVATE  ROOT::Core ROOT::Hist ROOT::Physics ROOT::Gpad)
target_link_libraries(fit_diodi PRIVATE  ROOT::Core ROOT::Hist ROOT::Physics ROOT::Gpad)

