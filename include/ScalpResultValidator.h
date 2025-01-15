//
// Created by anug on 11/14/23.
//

#ifndef MATRIXMULT_SCALPRESULTVALIDATOR_H
#define MATRIXMULT_SCALPRESULTVALIDATOR_H

#include "ScaLP/Solver.h"
#include "ScalpHandler.h"
#include "AdditionalFunc.h"
#include "ArgParser.h"

class ScalpResultValidator {
public:
    ScalpResultValidator(const std::map<ScaLP::Variable, double>  &res,
                         const std::map<std::tuple<int, int, int, int>,ScaLP::Variable> &PT_uvwn,
                         const std::map<std::string,ScaLP::Variable> &PT_List);

    void setValidatorArg(const std::map<std::string,std::string> &arg);
    void validate();

    std::map<std::tuple<int, int, int>, double> getTensorResult();


private:
    std::map<ScaLP::Variable, double> resultValues;
    std::map<std::tuple<int, int, int, int>, ScaLP::Variable> PT_uvwn_Variable;
    std::map<std::string,ScaLP::Variable> PT_List;
    double tensorSum;
    std::map<std::tuple<int, int, int>, double> tensorResult;
    int x;
    int y;
    int z;
    int N;
    bool validatorResult;
};


#endif //MATRIXMULT_SCALPRESULTVALIDATOR_H
