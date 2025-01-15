//
// Created by anug on 11/9/23.
//

#ifndef MATRIXMULT_ARGPARSER_H
#define MATRIXMULT_ARGPARSER_H

#include <map>
#include <functional>
#include <iostream>
#include "AdditionalFunc.h"
#include <stdexcept>
#include <tuple>

class ArgParser {
public:
    ArgParser(int &argc, char **argv);
    std::tuple<int, int, int>  getArg_sum_time_thread();


    //getter
    int getx();
    int gety();
    int getz();
    int getn();
    std::string getSolver();
    std::map<std::string, std::string> getArg();
    std::map<std::string, std::string> getParamList();




private:
    void showHelp();
    void showSolver();
    void setSum();
    void setThread();
    void setTimeout();
    void parse();
    void setSolver();
    void setLog();
    std::string getNextArgument();
    void setArgList();
    void setParam();

    int outputLog =0;
    int sumOpt =0;
    int solverTimeout = 0;
    int threadUsage = 0;
    std::string solver ="Gurobi"; //default solver is gurobi


    int argc;
    char **argv;
    std::map<std::string, std::string> solverList;
    std::map<std::string, std::function < void() >> argumentMap;
    std::map<std::string, std::string> argList;
    int i;
    int x;
    int y;
    int z;
    int n;

    std::string PT_TextInput ="0";
    std::map<std::string, std::string> paramList;



};

#endif //MATRIXMULT_ARGPARSER_H
