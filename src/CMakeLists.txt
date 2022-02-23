
FILE(GLOB MyCppSources ./*.cpp)
FILE(GLOB MyCppheaders ./*.h)

# TODO move the unit test to test build and include in test mechanims of cmake
#list(REMOVE_ITEM MyCppSources "./unitTestBasicTypes.cpp")
#list(REMOVE_ITEM MyCppSources "./unitTestConnectNeuralNetWiringFromGenome.cpp")
#list(REMOVE_ITEM MyCppSources "./unitTestGridVisitNeighborhood.cpp")

find_package(OpenCV REQUIRED)

include_directories( ${OpenCV_INCLUDE_DIRS} )

SET(GCC_COVERAGE_COMPILE_FLAGS "-O3 -Wall -fexceptions -fopenmp")
SET(GCC_COVERAGE_LINK_FLAGS    "-lpthread -O3 -lz -lgomp")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")
set( CMAKE_VERBOSE_MAKEFILE on )

add_executable(biosim4 ${MyCppSources} ${MyCppheaders})
target_link_libraries( biosim4 PUBLIC ${OpenCV_LIBS} )


install(TARGETS biosim4 DESTINATION bin)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


