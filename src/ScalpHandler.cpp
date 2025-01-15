//
// Created by lunug on 10/14/23.
//
#include <iostream>
#include "ScalpHandler.h"
#include <string>
#include <ScaLP/Solver.h>


void s_init::init(const std::string &ScalpSolver) {
    try {
        ScaLP::Solver s({ScalpSolver});
    }
    catch (ScaLP::Exception &e) {
        std::cout << "ScaLP exception: " << e.what() << std::endl;
    }
}

void s_var::setint(int a) {
    ScaLP::Variable a = ScaLP::newIntegerVariable("a");
}

void s_var::setbinary(bool b) {
    private_b=b;
    ScaLP::Variable x = ScaLP::newIntegerVariable("b");
}

