#include <iostream>
#include <exception>
#include "ScaLP/Solver.h"
#include "ScalpHandler.h"
#include "ScalpOutputHandler.h"
#include "ArgParser.h"
#include "ScalpResultValidator.h"
#include "ScalpResultFormulator.h"


int main(int argc, char** argv) {


    try {
        //Parse the arguments
        ArgParser Ar(argc,argv);

        //initialize Scalp object
        ScalpHandler s(Ar.getArg());


        //set parameter manually
        s.setParamList(Ar.getParamList());

        //set variables
        s.Set_Variables();

        //set constraints
        s.SetConstraint();

        //run solver
        s.RunSolver();


        //set output handler param
        ScalpOutputHandler Out(s.getResultValues(),s.getDuration(),s.getPT());
        Out.setArg(Ar.getArg());
        Out.setSolverData(s.getSolverData());
        Out.setParam(Ar.getParamList());

        //if its feasible then validate and formulate the result
        if(s.solverStat()){
            //Validate the result
            ScalpResultValidator validator(s.getResultValues(), s.getPT(), s.getPT_List());
            validator.setValidatorArg(Ar.getArg());
            validator.validate();

            //Formulate the result
            ScalpResultFormulator Formulator(s.getResultValues(), s.getPT());
            Formulator.setFormulatorArg(Ar.getArg());
            Formulator.setSABC(s.getSA(),s.getSB(),s.getSC());
            Formulator.setZABC(s.getZA(),s.getZB(),s.getZC());
            Formulator.formulateRank1Tensor();
            Out.setTensorResult(validator.getTensorResult());
            Out.setFormulation(Formulator.getRank1_U(),Formulator.getRank1_V(),Formulator.getRank1_W(),Formulator.getTotalNumberSum(),Formulator.getMapWforN());

        }


        //output the result
        Out.printResult();


    } catch(ScaLP::Exception& e){
        std::cerr << e.what() << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return 2;
    }
    catch (std::runtime_error& e) {
        std::cerr << e.what() << std::endl;
        return 3;
    }
    catch (std::domain_error &e) {
        std::cerr << e.what() << std::endl;
        return 4;
    }
    catch (...) {
        std::cerr << "Exception MatMul: something is broken" << std::endl;
        return 5;
    }

    std::cout << "MatMul is finished"<<std::endl;


    return 0;
}
