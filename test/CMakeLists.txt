cmake_minimum_required(VERSION 3.22.1)
project(BioSim4 VERSION 0.3.0)

# include(CTest)
# enable_testing()

set (
    exe_name
    bio_tests
)

set(
    sources
    ../src/common/test/test_dir.cpp
    ../src/common/test/test_compass.cpp
    ../src/common/test/test_coord.cpp
    ../src/common/test/test_polar.cpp
    ../src/common/test/test_grid.cpp
    ../src/common/test/test_column.cpp
    ../src/common/test/test_createGridBarrierVisitor.cpp
    ../src/common/test/test_gridBuilder.cpp
    ../src/common/test/test_layer.cpp
    ../src/common/test/test_gridCircle.cpp
    ../src/common/src/polar.cpp
    ../src/common/src/coord.cpp 
    ../src/common/src/dir.cpp
    ../src/common/src/column.cpp
    ../src/common/src/layer.cpp
    ../src/common/src/gridCircle.cpp
    ../src/common/src/gridRectangle.cpp
    ../src/common/src/grid.cpp
    ../src/common/src/createGridBarrierVisitor.cpp
    ../src/common/src/gridShapeFactory.cpp
    ../src/common/src/gridBuilder.cpp
    ../src/common/src/gridShape.cpp
    ../src/common/src/gridLocationVisitor.cpp 
    ../src/common/src/layer.cpp
    ../src/random.cpp
)

SET(GCC_COVERAGE_COMPILE_FLAGS "-O3 -Wall -fexceptions -fopenmp")
SET(GCC_COVERAGE_LINK_FLAGS    "-lpthread -O3 -lz -lgomp")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
set( CMAKE_VERBOSE_MAKEFILE on )

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.0.1 # or a later release
)

FetchContent_MakeAvailable(Catch2)

add_executable(${exe_name} ${sources})

target_link_libraries(
    ${exe_name}
    PRIVATE 
    Catch2::Catch2WithMain
)

install(TARGETS ${exe_name} DESTINATION bin)