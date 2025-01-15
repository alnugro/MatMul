#include <iostream>
#include <ScaLP/Solver.h>
#include <exception>
#include "ErrorHandler.h"
#include "ScalpHandler.h"

int main(int argc, char** argv) {

    if (argc != 5) { //restrict the input to 4 arguments
        std::cerr << "Usage: \nUse this format as an argument \"X Y Z N\" for matrix multiplication X.Y x Y.Z. for example \"./MatrixMult 2 2 3 6\" for 2x2 times 2x3 matrices constrained to 6 multiplication\n";
        return 1; //error code
    }

    int x = StoiPlusHandler::StringToInt(argv[1]);  //declaring the argument as Matrix size and multiplication count#include <iostream>
    int y = StoiPlusHandler::StringToInt(argv[2]);
    int z = StoiPlusHandler::StringToInt(argv[3]);
    int n = StoiPlusHandler::StringToInt(argv[3]);

    if (x <= 0 || y <= 0 || z <= 0 || n <= 0) {
        std::cerr << "Error occurred, X Y Z N have to be an int and they also have to be bigger than zero" << std::endl;
        return 1; //error code
    }
    //initialize Scalp
    s_init::init("gurobi");



    //ScaLP::newBinaryVariable()


    std::cout << "The test works"<<std::endl;




    return 0;
}
