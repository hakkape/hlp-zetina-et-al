# https://github.com/alberto-santini/cplex-example
# https://github.com/martinWANG2014/CplexCPlusPlusDemo
cmake_minimum_required(VERSION 3.22)

project(hlpsolver VERSION 1 
            DESCRIPTION "Library to use the HLP solver from Zetina et al. 2021"
            )
# Flag is necessary to deal with many variables being defined in the def.h file that is included multiple times...
set(CMAKE_C_FLAGS -fcommon)

# Where to find info on where to find cplex, then find it
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(CPLEX REQUIRED)

if(CPLEX_FOUND)
    include_directories(${CPLEX_INCLUDE_DIR})
    link_directories(${CPLEX_LIBRARY})
   message(STATUS "CPLEX found!")
else()
   message(STATUS "CPLEX not found!")
endif ()

if(CPLEX_ILOCPLEX_FOUND)
    include_directories(${CPLEX_ILOCPLEX_INCLUDE_DIR})
    link_directories(${CPLEX_ILOCPLEX_LIBRARY})
   message(STATUS "ILOCPLEX found!")
else()
   message(STATUS "ILOCPLEX not found!")
endif()



# Create the shared library that we can use with the python wrapper
set(hlplib_sources
    src/def.h
    src/Benders_framework.c
    src/Matheuristic.c
    src/Model_info.c
    src/ReadData.c
    src/USApHMPCsolver.c
    src/USApHMPCsolver.h
    src/hlp_reader.c
    src/hlp_reader.h)
add_library(hlp SHARED ${hlplib_sources})

# Link CPLEX
target_link_libraries(hlp cplex-library ilocplex)


# Create the executable
set(B4QFL_sources src/main.c)
add_executable(B4QFL ${B4QFL_sources})
target_link_libraries(B4QFL hlp)

# I am not sure if this is necessary, but we will link the libraries that are linked in the makefile
find_library(MATH_LIBRARY m)
if(MATH_LIBRARY)
    target_link_libraries(hlp ${MATH_LIBRARY})
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(hlp Threads::Threads)
target_link_libraries(hlp dl)