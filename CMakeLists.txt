cmake_minimum_required(VERSION 3.21)
project(hpc_disease_simulation)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
# include_directories(simulation)

add_executable(hpc_disease_simulation
        simulation/main.cpp
        #simulation/simulation.cpp
        )


add_executable(test
        simulation/test.cpp
        #simulation/simulation.cpp
        )