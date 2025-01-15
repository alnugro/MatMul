//
// Created by anug on 11/9/23.
//

#include "ArgParser.h"



ArgParser::ArgParser(int &argc, char** argv):argc(argc),argv(argv) {

    argumentMap["--help"] = std::bind(&ArgParser::showHelp, this);
    argumentMap["--sum"] = std::bind(&ArgParser::setSum, this);
    argumentMap["--thread"] = std::bind(&ArgParser::setThread, this);
    argumentMap["--timeout"] = std::bind(&ArgParser::setTimeout, this);
    argumentMap["--solver"] = std::bind(&ArgParser::setSolver, this);
    argumentMap["--showSolver"] = std::bind(&ArgParser::showSolver, this);
    argumentMap["--outlog"] = std::bind(&ArgParser::setLog, this);
    argumentMap["--param"] = std::bind(&ArgParser::setParam, this);


    solverList.insert({"Gurobi","Gurobi"});
    solverList.insert({"CPLEX","CPLEX"});
    solverList.insert({"LPSolve","LPSolve"});
    solverList.insert({"SCIP","SCIP"});

    //default parameter turn it off as needed
    paramList.insert({"OutputFlag","1"});
    paramList.insert({"LogFile","GurobiLog"});
    paramList.insert({"LogToConsole","0"});


    //enable --help as the first arg
    std::string arg= argv[1];
    if (argc == 2 && arg== "--help" ){
        argumentMap["--help"]();
    }

    else if (argc == 2 && arg== "--showSolver"){
        argumentMap["--showSolver"]();
    }

        //restrict the input to 4 arguments except for --help
    else if (argc < 5) {
        throw std::invalid_argument("Invalid Argument: Use this format as an argument \"X Y Z N\" for matrix multiplication X.Y x Y.Z. \nFor example \"./MatrixMult 2 2 3 6\" for 2x2 multiplied by 2x3 matrices constrained to 6 multiplication\nFollowed by the [--options] \ntype \"./MatrixMult --help\" for more info.");
    }

    x = AdditionalFunc::StringToInt(argv[1]);
    y = AdditionalFunc::StringToInt(argv[2]);
    z = AdditionalFunc::StringToInt(argv[3]);
    n = AdditionalFunc::StringToInt(argv[4]);


    if (x <= 0 || y <= 0 || z <= 0 || n <= 0) {
        throw std::invalid_argument ("Error occurred, X Y Z N have to be an int and they also have to be bigger than zero"); //error code
    }

    parse();

    setArgList();
}


void ArgParser::parse() {
    for (i = 5; i < argc; ++i) {
        std::string arg = argv[i];
        auto it = argumentMap.find(arg);
        if (it != argumentMap.end()) {
            // Call the function associated with the argument
            it->second();

        } else {
            throw std::invalid_argument("Unknown argument: " + arg + ". Use --help for usage information.");
        }
    }
}


void ArgParser::setSum() {
    sumOpt = 1;
//    std::cout << "sum is called, misum is set to: " << minSum << std::endl;
}


void ArgParser::setThread() {
    int iplus=i+1;
    if (iplus < argc) {
        threadUsage = AdditionalFunc::StringToInt(getNextArgument());
//        std::cout << "thread is called, thread is: " << threadUsage << std::endl;
    } else {
        throw std::invalid_argument("Exception: Set the value for thread" );
    }
}

void ArgParser::setTimeout() {
    int iplus=i+1;
    if (iplus < argc) {
        solverTimeout = AdditionalFunc::StringToInt(getNextArgument());
//        std::cout << "timeout is called, timeout is: " << solverTimeout << std::endl;
    } else {
        throw std::invalid_argument("Exception: Set the value for Timeout");
    }
}

void ArgParser::setParam() {
    int iplus=i+1;

    if (iplus > argc){
        std::cout<<"MatMul: No manually set parameter given"<< std::endl;
    }

    for (int j = iplus; j <= argc-1 ; j+=2){
        std::string par =argv[j];

        // Stop processing --setParam arguments when another -- argument is encountered
        if(par.find("--") == 0){
            throw std::invalid_argument("Exception: put --param at the end of the argument");
        }

        if (j>argc-2) {
            throw std::invalid_argument("Exception: Last param value doesn't exist");
        }

        std::string val =argv[j+1];
        if (val.find("--") == 0) {
            throw std::invalid_argument("Exception: put --param at the end of the argument");
        }


        paramList.insert({par,val});
    }



    //end i loop
    i = argc;
}


std::string ArgParser::getNextArgument() {
    // Return the next argument in the command line
    // Assuming there is at least one more argument
    return argv[++i];
}

std::tuple<int, int, int> ArgParser::getArg_sum_time_thread() {
    std::tuple argument = std::make_tuple(sumOpt, solverTimeout, threadUsage);
    return argument;
}

int ArgParser::getx() {
    return x;
}

int ArgParser::gety() {
    return y;
}

int ArgParser::getz() {
    return z;
}

int ArgParser::getn() {
    return n;
}

void ArgParser::showHelp() {
    std::cout << "Usage: ./MatrixMult [OPTIONS]\n"
              << "Options:\n"
              << "  --help           Show help message.\n"
              << "  --sum            Minimize the number of summation in the solver (default is off).\n"
              << "  --thread n       Set the amount of thread \"n\" the computer will use for the solver. \n"
              << "  --timeout n      Set the maximum length of time the solver will take in seconds.\n"
              << "  --solver         Set the solver backend you are using, default is gurobi. type --showSolver to see the supported solver available.\n"
              << "  --outlog         Do not Generate a folder called \"OutputFiles\" with an output text file within it in the current executable directory. (Default is on)\n"
              << "  --param          Set the the solver parameter you want to parse manually to the solver. you can put multiple parameter here separated by space. put this argument only at the end or behind all the arguments\n"
              << "  --param paramname paramvalue paramname2 paramvalue2       More example:   --param MIPFocus 1 SolutionLimit 2    \n";

    throw std::runtime_error ("End --help");
}

std::string ArgParser::getSolver() {
    return solver;
}

void ArgParser::setSolver() {
    int iplus=i+1;
    if(solverList.find(argv[iplus])!=solverList.end()) {
        std::cout << "MatMul: Solver is called, solver is now: " << solver << std::endl;
        if (iplus < argc) {
            solver = getNextArgument();
        } else {
            throw std::invalid_argument("Exception: Set the value for Timeout");
        }
    }
    else {
        throw std::invalid_argument("Solver is not supported");
    }
}


void ArgParser::showSolver() {
    std::cout << "MatMul: Supported Solver: " << std::endl;
    for (const auto& [key, value] : solverList) {
        std::cout << " -" << key << std::endl;
    }
    throw std::runtime_error ("End --showSolver");
}

void ArgParser::setLog() {
    outputLog = true;
}

void ArgParser::setArgList() {
    argList.insert({"solver",solver});
    argList.insert({"x",std::to_string(x)});
    argList.insert({"y",std::to_string(y)});
    argList.insert({"z",std::to_string(z)});
    argList.insert({"N",std::to_string(n)});
    argList.insert({"sumOpt",std::to_string(sumOpt)});
    argList.insert({"thread",std::to_string(threadUsage)});
    argList.insert({"timeout",std::to_string(solverTimeout)});
    argList.insert({"outlog",std::to_string(outputLog)});
}

std::map<std::string, std::string> ArgParser::getArg() {
    return argList;
}

std::map<std::string, std::string> ArgParser::getParamList() {
    return paramList;
}







