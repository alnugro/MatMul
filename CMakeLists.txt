cmake_minimum_required(VERSION 3.0)
project(MatrixMult)

set(CMAKE_CXX_STANDARD 17)

# Deactivate C++11-abi?
if(OLD_CXX_ABI) 
  add_definitions(-D_GLIBCXX_USE_CXX11_ABI=0) 
endif()

# find ScaLP
find_library(SCALP_LIB
        NAMES libScaLP.so libScaLP.dylib
        DOC "Directory of the ScaLP library")
FIND_PATH(SCALP_H
        ScaLP/Solver.h
        DOC "Path to main ScaLP header file Solver.h"
        )

IF (SCALP_LIB AND SCALP_H)
    message(STATUS "Found ScaLP: ${SCALP_LIB} and ${SCALP_H}")
    add_definitions(-DUSE_SCALP)
ELSE (SCALP_LIB AND SCALP_H)
    message(STATUS "Could not find ScaLP (${SCALP_LIB} and ${SCALP_H}). Please specify path by including it in CMAKE_PREFIX_PATH (-DCMAKE_PREFIX_PATH=...)")
ENDIF (SCALP_LIB AND SCALP_H)



#add executable for Matrix multiplication
add_executable(MatrixMult
        src/main.cpp
        src/AdditionalFunc.cpp
        src/ScalpHandler.cpp
        src/ScalpOutputHandler.cpp include/ScalpOutputHandler.h src/ArgParser.cpp include/ArgParser.h src/ScalpResultValidator.cpp include/ScalpResultValidator.h src/ScalpResultFormulator.cpp include/ScalpResultFormulator.h)

#Link filesystem lib
target_link_libraries(MatrixMult PRIVATE stdc++fs)

#include header files for Matrix multiplication
target_include_directories(MatrixMult PRIVATE include)

#include the header files for scalp
if(SCALP_H)
    target_include_directories(MatrixMult PUBLIC ${SCALP_H})
ENDIF(SCALP_H)

if(SCALP_LIB)
    target_link_libraries(MatrixMult PUBLIC ${SCALP_LIB})
ENDIF(SCALP_LIB)
