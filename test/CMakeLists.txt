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
    ../src/common/src/polar.cpp
    ../src/common/src/coord.cpp 
    ../src/common/src/dir.cpp
    ../src/common/src/column.cpp
    ../src/grid.cpp
)

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