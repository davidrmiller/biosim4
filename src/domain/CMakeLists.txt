cmake_minimum_required(VERSION 3.22.1)
project(BioSim4 VERSION 0.3.0)

set (
    lib_name
    biosim4_domain
)

FILE(GLOB sources ./src/*.cpp)

add_library(${lib_name} ${sources})