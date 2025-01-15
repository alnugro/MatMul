//
// Created by anug on 11/6/23.
//

#ifndef MATRIXMULT_SCALPOUTPUTHANDLER_H
#define MATRIXMULT_SCALPOUTPUTHANDLER_H


#include "ScaLP/Solver.h"
#include <cmath>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <iomanip>
#include <map>
#include <string>
#include "AdditionalFunc.h"

class ScalpOutputHandler{
public:
    ScalpOutputHandler(const std::map<ScaLP::Variable, double>  &res ,const std::chrono::duration<double> &SolverDuration ,const std::map<std::tuple<int, int, int, int>,  ScaLP::Variable> &PT_uvwn);
    void printResult();


    //setter
    void setArg( const std::map<std::string, std::string> & arg);
    void setSolverData(const std::map<std::string, std::string> & solverData);
    void setTensorResult(const std::map<std::tuple<int, int, int>, double> & tensorResult);
    void setFormulation(const std::map<std::tuple<int, int>,double>&,
                        const std::map<std::tuple<int, int>,double>&,
                        const std::map<std::tuple<int, int>,double>&,
                        const int &, const std::map<std::string, std::vector<int>> &);

    void setParam(const std::map<std::string , std::string> & para);


private:
    std::map<ScaLP::Variable, double>  refResultValues;
    std::chrono::duration<double> refSolverDuration;
    std::map<std::tuple<int, int, int, int>, ScaLP::Variable> refPT_uvwn_Variable;
    bool outputLog = false;
    bool sumOpt = false;
    std::string solverTimeout;
    std::string threadUsage;
    std::string solver;
    std::map<std::tuple<int, int, int>, double> refTensorResult;
    int x;
    int y;
    int z;
    int N;
    std::string refTotalNumberVariables;
    std::string refTotalNumberConstraints;
    std::string solverStatus;
    std::string solverFeasibility;

    std::map<std::tuple<int, int>,double> rank1tensor_U;
    std::map<std::tuple<int, int>,double> rank1tensor_V;
    std::map<std::tuple<int, int>,double> rank1tensor_W;

    std::map<std::string , std::vector<int>> mapM;
    std::map<std::string , std::string> paramList;

    int totalSum;

    void resultDetails();



};

#endif //MATRIXMULT_SCALPOUTPUTHANDLER_H

