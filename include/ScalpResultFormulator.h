//
// Created by anug on 11/15/23.
//

#ifndef MATRIXMULT_SCALPRESULTFORMULATOR_H
#define MATRIXMULT_SCALPRESULTFORMULATOR_H

#include "ScaLP/Solver.h"
#include "AdditionalFunc.h"
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cmath>





class ScalpResultFormulator {
public:
    ScalpResultFormulator(const std::map<ScaLP::Variable, double>  &res,
                          const std::map<std::tuple<int, int, int, int>,ScaLP::Variable> &PT_uvwn);

    void setFormulatorArg(const std::map<std::string,std::string> &arg);
    void setSABC(
           const std::map<std::tuple<int, int>, ScaLP::Variable> &SA_un_Variable,
           const std::map<std::tuple<int, int>, ScaLP::Variable> &SB_vn_Variable,
           const std::map<std::tuple<int, int>, ScaLP::Variable> &SC_wn_Variable);

    void setZABC(const std::map<std::tuple<int, int>, ScaLP::Variable> &ZA_un_Variable,
                                        const std::map<std::tuple<int, int>, ScaLP::Variable> &ZB_vn_Variable,
                                        const std::map<std::tuple<int, int>, ScaLP::Variable> &ZC_wn_Variable);


    void formulateRank1Tensor();

    //getter
     std::map<std::tuple<int, int>,double> getRank1_U();
     std::map<std::tuple<int, int>,double> getRank1_W();
     std::map<std::tuple<int, int>,double> getRank1_V();
     std::map<std::string, std::vector<int>> getMapWforN();
     [[nodiscard]] int getTotalNumberSum() const;



private:
    std::map<ScaLP::Variable, double> resultValues;
    std::map<std::tuple<int, int, int, int>, ScaLP::Variable> PT_uvwn_Variable;
    std::map<std::tuple<int, int>,double> valueRank1tensor_U;
    std::map<std::tuple<int, int>,double> valueRank1tensor_V;
    std::map<std::tuple<int, int>,double> valueRank1tensor_W;

    std::map<std::tuple<int, int>, ScaLP::Variable> SA;
    std::map<std::tuple<int, int>, ScaLP::Variable> SB;
    std::map<std::tuple<int, int>, ScaLP::Variable> SC;

    std::map<std::tuple<int, int>, ScaLP::Variable> ZA;
    std::map<std::tuple<int, int>, ScaLP::Variable> ZB;
    std::map<std::tuple<int, int>, ScaLP::Variable> ZC;

    std::map<std::string, std::vector<int>> mapWforN;

    int x;
    int y;
    int z;
    int N;
    int totalNumberSum;


    void setRank1tensor(int & u, int & v, int & w, int &n);

    void insertTensor_U(std::tuple<int,int> , double);
    void insertTensor_V(std::tuple<int,int> , double);
    void insertTensor_W(std::tuple<int,int> , double);

    void equationExtractor();
    void sumCounter();

};


#endif //MATRIXMULT_SCALPRESULTFORMULATOR_H
