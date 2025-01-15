//
// Created by lunug on 10/14/23.
//
#include <string>
#include <chrono>
#include <iostream>
#include <ctime>
#include "ScaLP/Solver.h"
#include <filesystem>
#include <tuple>
#include "AdditionalFunc.h"



#ifndef MATRIXMULT_SCALPHANDLER_H
#define MATRIXMULT_SCALPHANDLER_H

class ScalpHandler {
public:
    ScalpHandler(int x, int y, int z, int n, const std::string& ScalpSolver);
    explicit ScalpHandler(const std::map<std::string, std::string> &arg);
    void Set_Variables();
    void SetConstraint();
    void SetPT_SumList();
    void SetPT_Sum();
    void setParamList(const std::map<std::string, std::string> & para);
    void RunSolver();
    void setArgOption(const std::map<std::string, std::string> & arg);
    void sumOptimizer();
    bool solverStat();

    //getter
    std::chrono::duration<double> getDuration();
    std::map<ScaLP::Variable, double>  getResultValues();
    std::map<std::tuple<int, int, int, int>, ScaLP::Variable> getPT();
    std::map<std::string, std::string> getSolverData();
    std::map<std::string,ScaLP::Variable> getPT_List();
    std::map<std::tuple<int, int>, ScaLP::Variable> getSA();
    std::map<std::tuple<int, int>, ScaLP::Variable> getSB();
    std::map<std::tuple<int, int>, ScaLP::Variable> getSC();

    std::map<std::tuple<int, int>, ScaLP::Variable> getZA();
    std::map<std::tuple<int, int>, ScaLP::Variable> getZB();
    std::map<std::tuple<int, int>, ScaLP::Variable> getZC();






private:
    void betterSetConstraint();
    std::chrono::high_resolution_clock::time_point startTime;
    std::chrono::high_resolution_clock::time_point endTime;
    std::chrono::duration<double> solverDuration;
    ScaLP::Solver solver;
    ScaLP::Result result;
    ScaLP::status status;
    std::map<ScaLP::Variable, double> resultValues;

    std::map<std::tuple<int, int>, ScaLP::Variable> SA_un_Variable;
    std::map<std::tuple<int, int>, ScaLP::Variable> SB_vn_Variable;
    std::map<std::tuple<int, int>, ScaLP::Variable> SC_wn_Variable;

    std::map<std::tuple<int, int>, ScaLP::Variable> ZA_un_Variable;
    std::map<std::tuple<int, int>, ScaLP::Variable> ZB_vn_Variable;
    std::map<std::tuple<int, int>, ScaLP::Variable> ZC_wn_Variable;
    std::map<std::tuple<int, int, int, int>, ScaLP::Variable> ST_uvwn_Variable;
    std::map<std::tuple<int, int, int, int>, ScaLP::Variable> ZT_uvwn_Variable;
    std::map<std::tuple<int, int, int, int>, ScaLP::Variable> PT_uvwn_Variable;
    std::map<std::tuple<int, int, int>, ScaLP::Term>PT_Sum;
    ScaLP::Variable z_bin;
    ScaLP::Term sumOptimizerTerm;
    std::map<std::string,ScaLP::Variable> PT_List;
    int x,y,z,N;
    bool sumOpt;

    std::map<std::string, std::string> paramList;


    std::map<std::string,std::string> solverData;
    std::size_t totalNumberVariables= 0;
    std::size_t totalNumberConstraints=0;

    std::string solverStatus;
    std::string solverFeasibility;



};








#endif //MATRIXMULT_SCALPHANDLER_H
