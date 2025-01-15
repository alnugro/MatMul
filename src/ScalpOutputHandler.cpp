////
//// Created by anug on 11/6/23.
////

#include "ScalpOutputHandler.h"





ScalpOutputHandler::ScalpOutputHandler(const std::map<ScaLP::Variable, double>  &res , const std::chrono::duration<double> &SolverDuration, const std::map<std::tuple<int, int, int, int>, ScaLP::Variable> &PT_uvwn):
        refResultValues(res),
        refSolverDuration(SolverDuration),
        refPT_uvwn_Variable(PT_uvwn) {}

void ScalpOutputHandler::setArg(const std::map<std::string, std::string> &arg) {
    try {
        if (arg.at("sumOpt") == "1") {
            sumOpt = true;
        }

        if (arg.at("outlog") != "0") {
           outputLog = true;
        }
        threadUsage = arg.at("thread");
        solverTimeout = arg.at("timeout");
        solver = arg.at("solver");
        x =AdditionalFunc::StringToInt(arg.at("x"));
        y =AdditionalFunc::StringToInt(arg.at("y"));
        z =AdditionalFunc::StringToInt(arg.at("z"));
        N =AdditionalFunc::StringToInt(arg.at("N"));


    } catch (const std::out_of_range &ex) {
        // Handle the case where a key is not found in the map
        throw std::runtime_error ("Exception ScalpOutputHandler: Cannot find Argument");
    }
}


void ScalpOutputHandler::printResult() {
    //Exclude the Logger when true
    if (outputLog){
        return;
    }
    //Set the time now
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm* timeInfo = std::localtime(&currentTime);

    std::ostringstream oss;
    oss << std::put_time(timeInfo, "%H%M%S_%Y_%m_%d");
    std::string TimeString = oss.str();
    //Creating Directory
    std::filesystem::create_directory("OutputFiles");

    //create txt output
    std::string fileName = "OutputFiles/"+ std::to_string(x) + std::to_string(y) + std::to_string(z) + std::to_string(N) + "_" + TimeString + ".txt";
//    std::string fileName = "OutputFiles/test";

    //Working on Output File
    std::ofstream outputFile;
    outputFile.open(fileName);

    if (outputFile.is_open()) {
        outputFile << "MatrixMult ScaLP by Achmad Luthfi Nugroho"<<std::endl;
        outputFile << "*************************************"<< std::endl;
        outputFile << "Matrix A (X * Y) -> (" << x << " * " << y << "):" << std::endl;
        outputFile << "Matrix B (Y * Z) -> (" << y << " * " << z << "):" << std::endl;
        outputFile << "*************************************\n"<< std::endl;

        // Sort Tensor size in descending order
        int tensorSize[] = {x, y, z};
        std::sort(std::begin(tensorSize), std::end(tensorSize), std::greater<>());

        outputFile << "The Given Matrix" << std::endl;
        outputFile << "*************************************\n";
        for(int r= 0; r < tensorSize[0]; r++){
            outputFile << "| ";
            if(r<x){
                for(int a = 1; a <= y; a++) {
                    outputFile << "A" <<a+r*y << " ";
                }
            }
            else {
                for(int a = 1; a <= y; a++) {
                    outputFile << "   ";
                }
            }
            outputFile << "|";
            outputFile << "    x    ";
            outputFile << "| ";
            if(r<y){
                for(int b = 1; b <= z; b++){
                    outputFile << "B" <<b+r*z<< " ";
                }
            }
            else {
                for(int b = 1; b <= z; b++){
                    outputFile << "   ";
                }
            }
            outputFile << "|";
            outputFile << "    =    ";
            outputFile << "| ";
            if(r<x){
                for(int c= 1; c <= z; c++){
                    outputFile << "C" <<c+r*z<< " ";
                }
            }
            else {
                for(int c= 1; c <= z; c++){
                    outputFile << "   ";
                }
            }
            outputFile << "|";
            outputFile << "\n";


        }
        outputFile << "*************************************\n\n"<< std::endl;


        outputFile << "Solver Result Details" << std::endl;
        outputFile << "*************************************"<< std::endl;
        outputFile << "  Solver Feasibility         : " << solverFeasibility <<std::endl;
        outputFile << "  Solver Status              : "<< solverStatus<<std::endl;
        outputFile << "  Number of Summation        : " << totalSum <<std::endl;
        outputFile << "  Multiplication size N of   : " << N << std::endl;
        outputFile << "*************************************\n"<< std::endl;

        //if its feasible show the result
        if(solverFeasibility=="feasible") {

            outputFile << "Solver Equation Result" << std::endl;
            outputFile << "*************************************" << std::endl;
            for (int w = 0; w <= (x * z) - 1; ++w) {
                std::string key = std::to_string(w);
                outputFile << "C" << w + 1 << "= ";
                if (!mapM[key].empty()) {
                    auto iter = mapM[key].begin();
                    std::string signM;
                    if (rank1tensor_W.find({w, *iter}) != rank1tensor_W.end()) {
                        int signMInt = std::round(rank1tensor_W.at({w, *iter}));
                        signM = std::to_string(signMInt);
                    }
                    outputFile << "(" << signM << ")" << "M" << *iter + 1;
                    ++iter;

                    for (; iter != mapM[key].end(); ++iter) {
                        if (rank1tensor_W.find({w, *iter}) != rank1tensor_W.end()) {
                            int signMInt = std::round(rank1tensor_W.at({w, *iter}));
                            signM = std::to_string(signMInt);
                        }
                        outputFile << " + (" << signM << ")" << "M" << *iter + 1;
                    }
                } else {
                    outputFile << "Exception: MapM cant find C" << w + 1 << std::endl;
                }
                outputFile << "\n";
            }
            outputFile << std::endl;


            for (int nm = 0; nm <= N - 1; ++nm) {
                outputFile << "M" << nm + 1 << " = ";
                bool printedSomething = false;
                for (int u = 0; u <= (x * y) - 1; ++u) {
                    if (std::round(rank1tensor_U.at({u, nm})) == 1 || std::round(rank1tensor_U.at({u, nm})) == -1) {
                        if (printedSomething) {
                            outputFile << " + ";
                        }
                        outputFile << "(" << rank1tensor_U.at({u, nm}) << ")" << "A" << (u + 1);
                        printedSomething = true;
                    }
                }
                outputFile << " x ";

                printedSomething = false;
                for (int v = 0; v <= (y * z) - 1; ++v) {
                    if (std::round(rank1tensor_V.at({v, nm})) == 1 || std::round(rank1tensor_V.at({v, nm})) == -1) {
                        if (printedSomething) {
                            outputFile << " + ";
                        }
                        outputFile << "(" << rank1tensor_V.at({v, nm}) << ")" << "B" << (v + 1);
                        printedSomething = true;
                    }
                }
                outputFile << "\n";

            }

            outputFile << "*************************************\n" << std::endl;


            outputFile << "Rank 1 Tensor" << std::endl;
            outputFile << "*************************************";
            for (int nt = 0; nt < N; nt++) {
                outputFile << "\nM: " << nt + 1 << std::endl;
                outputFile << "-------------------------------------\n";
                outputFile << "  A     x       B      x      C" << std::endl;


                for (int i = 0; i < tensorSize[0] * tensorSize[1]; i++) {
                    if (rank1tensor_U.find({i, nt}) != rank1tensor_U.end()) {
                        outputFile << "| " << rank1tensor_U.at({i, nt}) << " |";
                    } else {
                        outputFile << "|  " << " |";
                    }
                    if (rank1tensor_V.find({i, nt}) != rank1tensor_V.end()) {
                        outputFile << "         | " << rank1tensor_V.at({i, nt}) << " |";
                    } else {
                        outputFile << "         | " << "  |";
                    }
                    if (rank1tensor_W.find({i, nt}) != rank1tensor_W.end()) {
                        outputFile << "         | " << rank1tensor_W.at({i, nt}) << " |\n";
                    } else {
                        outputFile << "         | " << "  |\n";
                    }
                }
                outputFile << "-------------------------------------\n";
            }
            outputFile << "*************************************\n\n\n" << std::endl;


            outputFile << "" << std::endl;
            outputFile << "Matrix Tensor" << std::endl;
            outputFile << "*************************************" << std::endl;
            outputFile << "Horizontal: A1,A2,A3... from left to right." << std::endl;
            outputFile << "Vertical: B1,B2,B3... from up to down.\n" << std::endl;


            for (int n = 0; n < N; n++) {
                bool neverPrint = true;
                outputFile << "Matrix Tensor N = " << n + 1 << std::endl;
                outputFile << "-------------------------------------\n";
                for (int w = 0; w < x * z; w++) {


                    outputFile << "C" << w + 1 << "      ";
                    for (int u = 0; u < x * y; u++) {
                        outputFile << " ";

                    }

                }

                outputFile << "\n";

                for (int v = 0; v < y * z; v++) {
                    for (int w = 0; w < x * z; w++) {
                        for (int u = 0; u < x * y; u++) {

                            if (refPT_uvwn_Variable.find({u, v, w, n}) != refPT_uvwn_Variable.end()) {
                                if (refResultValues.find(ScalpOutputHandler::refPT_uvwn_Variable.at({u, v, w, n})) !=
                                    refResultValues.end()) {
                                    outputFile << refResultValues.at(
                                            ScalpOutputHandler::refPT_uvwn_Variable.at({u, v, w, n}));
                                }
                            } else {
                                outputFile << " ";
                            }

                        }
                        outputFile << "        ";
                    }
                    outputFile << "" << std::endl;
                }
                outputFile << "-------------------------------------\n\n";
            }

            outputFile << "Tensor Sum result" << std::endl;
            outputFile << "*************************************" << std::endl;


            for (int w = 0; w < x * z; w++) {
                outputFile << "C" << w + 1 << "      ";
                for (int u = 0; u < x * y; u++) {
                    outputFile << " ";

                }
            }
            outputFile << "\n";

            for (int v = 0; v < y * z; v++) {
                for (int w = 0; w < x * z; w++) {
                    for (int u = 0; u < x * y; u++) {
                        if (refTensorResult.find({u, v, w}) != refTensorResult.end()) {
                            outputFile << refTensorResult.at({u, v, w});
                        } else {
                            outputFile << " ";
                        }
                    }
                    outputFile << "        ";
                }
                outputFile << "" << std::endl;
            }
            outputFile << "*************************************\n\n\n" << std::endl;

        }


        outputFile << "Solver Calculation Details" << std::endl;
        outputFile << "*************************************"<< std::endl;
        outputFile << "  Total number of Variables                  : " << refTotalNumberVariables << std::endl;
        outputFile << "  Total number of constraints in the solver  : " << refTotalNumberConstraints << std::endl;
        outputFile << "  Calculation Duration                       : " << refSolverDuration.count() <<"s" <<std::endl;
        outputFile << "*************************************\n"<< std::endl;



        outputFile << "Solver options" << std::endl;
        outputFile << "************************************* "<< std::endl;
        outputFile << "  Solver Backend       : " << solver <<std::endl;
        outputFile << "  Sum optimizer        : " << sumOpt <<" (1 = true, 0 = false)" <<std::endl;
        outputFile << "  Thread usage         : " << threadUsage <<std::endl;
        outputFile << "  Solver Timeout       : " << solverTimeout <<std::endl;
        outputFile << "************************************* \n"<< std::endl;

        outputFile << "Manually assigned Parameter to parse directly to solver" << solverTimeout <<std::endl;
        outputFile << "*************************************"<< std::endl;
        for(auto const & it:paramList){
            outputFile <<it.first<<" is set to " << it.second<<std::endl;
        }
        outputFile << "************************************* \n"<< std::endl;






        outputFile.close();
        std::cout << "MatMul: Output data written to file successfully.\n";
    } else {
        std::cerr << "MatMul: Error opening the file.\n";
    }

}

void ScalpOutputHandler::setSolverData(const std::map<std::string, std::string> &solverData) {
    refTotalNumberConstraints = solverData.at("totalConstraints");
    refTotalNumberVariables = solverData.at("totalVariables");
    solverStatus = solverData.at("status");
    solverFeasibility = solverData.at("feasibility");
}



void ScalpOutputHandler::setTensorResult(const std::map<std::tuple<int, int, int>, double> &tensorResult) {
    refTensorResult = tensorResult;
}

void
ScalpOutputHandler::setFormulation(const std::map<std::tuple<int, int>, double> & u,
                                   const std::map<std::tuple<int, int>, double> & v,
                                   const std::map<std::tuple<int, int>, double> & w,
                                   const int &sum,
                                   const std::map<std::string , std::vector<int>> & map
                                   ) {
    rank1tensor_U = u;
    rank1tensor_V = v;
    rank1tensor_W = w;
    totalSum = sum;
    mapM = map;

}

void ScalpOutputHandler::resultDetails() {

}

void ScalpOutputHandler::setParam(const std::map<std::string, std::string> &para) {
    paramList=para;
}







