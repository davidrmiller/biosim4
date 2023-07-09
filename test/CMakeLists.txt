cmake_minimum_required(VERSION 3.22.1)
project(BioSim4 VERSION 0.3.0)

include(CTest)
enable_testing()

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
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/refs/tags/v1.13.0.zip
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(${exe_name} ${sources})

target_link_libraries(
    ${exe_name}
    gtest_main
)

install(TARGETS ${exe_name} DESTINATION bin)