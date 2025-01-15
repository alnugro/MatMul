//
// Created by lunug on 10/14/23.
//

#include "ScalpHandler.h"


ScalpHandler::ScalpHandler(int x, int y, int z, int N, const std::string& ScalpSolver):
x(x),
y(y),
z(z),
N(N),
solver({ScalpSolver})
{}


ScalpHandler::ScalpHandler(const std::map<std::string, std::string> &arg):
x(AdditionalFunc::StringToInt(arg.at("x"))),
y(AdditionalFunc::StringToInt(arg.at("y"))),
z(AdditionalFunc::StringToInt(arg.at("z"))),
N(AdditionalFunc::StringToInt(arg.at("N"))),
solver({arg.at("solver")})
{
    //set the additional options
    setArgOption(arg);
    startTime = std::chrono::high_resolution_clock::now();

}


void ScalpHandler::setArgOption(const std::map<std::string, std::string> &arg) {
    try {
        if (arg.at("sumOpt") == "1") {
            sumOpt = true;
        }

        if (arg.at("timeout") != "0") {
            solver.timeout = AdditionalFunc::StringToInt(arg.at("timeout"));
        }


        if (arg.at("thread") != "0") {
            solver.threads = AdditionalFunc::StringToInt(arg.at("thread"));
        }



    } catch (const std::out_of_range &ex) {
        // Handle the case where a key is not found in the map
        throw std::runtime_error ("Exception ScalpHandler: Cannot find Argument");
    }
}


void ScalpHandler::Set_Variables() {
    this->z_bin = ScaLP::newBinaryVariable("z_bin");

    for(int u = 0; u <= (x*y)-1; ++u) {
        for(int v = 0; v <= (y*z)-1; ++v) {
            for(int w = 0; w <= (x*z)-1; ++w) {
                for(int n = 0; n <= N-1; ++n) {

                    this->ST_uvwn_Variable[{u, v, w, n}] = ScaLP::newBinaryVariable("ST_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w) + "_" + std::to_string(n));
                    this->ZT_uvwn_Variable[{u, v, w, n}] = ScaLP::newBinaryVariable("ZT_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w) + "_" + std::to_string(n));
//                    std::cout << "ST = "<< this->ST_uvwn_Variable[{u, v, w, n}] << std::endl;
//                    std::cout << "ZT = "<< this->ZT_uvwn_Variable[{u, v, w, n}] << std::endl;

                    this->PT_uvwn_Variable[{u, v, w, n}] = ScaLP::newIntegerVariable("PT_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w) + "_" + std::to_string(n),-1,1);
//                    std::cout << "PT = "<< this->PT_uvwn_Variable[{u, v, w, n}] << std::endl;

                }
            }

        }
    }

    for(int u = 0; u <= (x*y)-1; ++u) {
        for(int n = 0; n <= N-1; ++n) {
            this->SA_un_Variable[{u, n}] = ScaLP::newBinaryVariable("SA_" + std::to_string(u) + "_" + std::to_string(n));
            this->ZA_un_Variable[{u, n}] = ScaLP::newBinaryVariable("ZA_" + std::to_string(u) + "_" + std::to_string(n));
            //std::cout << "SA = " << this->SA_un_Variable[{u, n}] << std::endl;
            //std::cout << "ZA = "<< this->ZA_un_Variable[{u, n}] << std::endl;
        }
    }
    for(int v = 0; v <= (y*z)-1; ++v) {
            for(int n = 0; n <= N-1; ++n) {
                this->SB_vn_Variable[{v, n}] = ScaLP::newBinaryVariable("SB_" + std::to_string(v) + "_" + std::to_string(n));
                this->ZB_vn_Variable[{v, n}] = ScaLP::newBinaryVariable("ZB_" + std::to_string(v) + "_" + std::to_string(n));
//                    std::cout << "SB = "<< this->SB_vn_Variable[{v, n}] << std::endl;
//                    std::cout << "ZB = "<< this->ZB_vn_Variable[{v, n}] << std::endl;
            }
    }

    for(int w = 0; w <= (x*z)-1; ++w) {
        for(int n = 0; n <= N-1; ++n) {
            this->SC_wn_Variable[{w, n}] = ScaLP::newBinaryVariable("SC_" + std::to_string(w) + "_" + std::to_string(n));
            this->ZC_wn_Variable[{w, n}] = ScaLP::newBinaryVariable("ZC_" + std::to_string(w) + "_" + std::to_string(n));
//                    std::cout << "SC = "<< this->SC_wn_Variable[{w, n}] << std::endl;
//                    std::cout << "ZC = "<< this->ZC_wn_Variable[{w, n}] << std::endl;
        }
    }
}

void ScalpHandler::SetConstraint() {
    for(int u = 0; u <= (x*y)-1; ++u) {
        for(int v = 0; v <= (z*y)-1; ++v) {
            for(int w = 0; w <= (x*z)-1; ++w) {
                for(int n = 0; n <= N-1; ++n) {




                    //Equation 3-6
                    this->solver.addConstraint(this->ZT_uvwn_Variable.at({u, v, w, n}) + this->PT_uvwn_Variable.at({u, v, w, n}) <= 1.0);
                    this->solver.addConstraint(this->ZT_uvwn_Variable.at({u, v, w, n}) - this->PT_uvwn_Variable.at({u, v, w, n}) <= 1.0);
                    this->solver.addConstraint(-2.0 * this->ST_uvwn_Variable.at({u, v, w, n}) - this->ZT_uvwn_Variable.at({u, v, w, n}) - this->PT_uvwn_Variable.at({u, v, w, n}) <= -1.0);
                    this->solver.addConstraint(2.0 * this->ST_uvwn_Variable.at({u, v, w, n}) - this->ZT_uvwn_Variable.at({u, v, w, n}) + this->PT_uvwn_Variable.at({u, v, w, n}) <= 1.0);

//                    this->solver.addConstraint(this->ZT_uvwn_Variable.at({u, v, w, n}) + this->PT_uvwn_Variable.at({u, v, w, n}) <= 1.0);
//                    this->solver.addConstraint(this->ZT_uvwn_Variable.at({u, v, w, n}) - this->PT_uvwn_Variable.at({u, v, w, n}) <= 1.0);
//                    this->solver.addConstraint(-2.0 * this->ST_uvwn_Variable.at({u, v, w, n}) - this->ZT_uvwn_Variable.at({u, v, w, n}) - this->PT_uvwn_Variable.at({u, v, w, n}) == -1.0);



                    this->solver.addConstraint(this->SA_un_Variable.at({u, n}) + this->SB_vn_Variable.at({v, n}) + this->SC_wn_Variable.at({w, n}) - this->ST_uvwn_Variable.at({u, v, w, n}) -2*z_bin == 0);






                    //equation 7-14
//                    this->solver.addConstraint(this->SA_un_Variable.at({u, n}) + this->SB_vn_Variable.at({v, n}) + this->SC_wn_Variable.at({w, n}) - this->ST_uvwn_Variable.at({u, v, w, n}) >= 0);
//                    this->solver.addConstraint(this->SA_un_Variable.at({u, n}) + this->SB_vn_Variable.at({v, n}) - this->SC_wn_Variable.at({w, n}) + this->ST_uvwn_Variable.at({u, v, w, n}) >= 0);
//                    this->solver.addConstraint(this->SA_un_Variable.at({u, n}) - this->SB_vn_Variable.at({v, n}) + this->SC_wn_Variable.at({w, n}) + this->ST_uvwn_Variable.at({u, v, w, n}) >= 0);
//                    this->solver.addConstraint(this->SA_un_Variable.at({u, n}) - this->SB_vn_Variable.at({v, n}) - this->SC_wn_Variable.at({w, n}) - this->ST_uvwn_Variable.at({u, v, w, n}) >= -2.0);
//                    this->solver.addConstraint(-1*this->SA_un_Variable.at({u, n}) + this->SB_vn_Variable.at({v, n}) + this->SC_wn_Variable.at({w, n}) + this->ST_uvwn_Variable.at({u, v, w, n}) >= 0);
//                    this->solver.addConstraint(-1*this->SA_un_Variable.at({u, n}) + this->SB_vn_Variable.at({v, n}) - this->SC_wn_Variable.at({w, n}) - this->ST_uvwn_Variable.at({u, v, w, n}) >= -2.0);
//                    this->solver.addConstraint(-1*this->SA_un_Variable.at({u, n}) - this->SB_vn_Variable.at({v, n}) + this->SC_wn_Variable.at({w, n}) - this->ST_uvwn_Variable.at({u, v, w, n}) >= -2.0);
//                    this->solver.addConstraint(-1*this->SA_un_Variable.at({u, n}) - this->SB_vn_Variable.at({v, n}) - this->SC_wn_Variable.at({w, n}) + this->ST_uvwn_Variable.at({u, v, w, n}) >= -2.0);

                    //equation 15-18
                    this->solver.addConstraint(this->ZA_un_Variable.at({u, n}) + this->ZB_vn_Variable.at({v, n}) + this->ZC_wn_Variable.at({w, n}) - this->ZT_uvwn_Variable.at({u, v, w, n}) >= 0);
                    this->solver.addConstraint(-1*this->ZA_un_Variable.at({u, n}) + this->ZT_uvwn_Variable.at({u, v, w, n}) >= 0);
                    this->solver.addConstraint(-1*this->ZB_vn_Variable.at({v, n}) + this->ZT_uvwn_Variable.at({u, v, w, n}) >= 0);
                    this->solver.addConstraint(-1*this->ZC_wn_Variable.at({w, n}) + this->ZT_uvwn_Variable.at({u, v, w, n}) >= 0);

//                    this->solver.addConstraint(this->ZA_un_Variable.at({u, n}) + this->ZB_vn_Variable.at({v, n}) + this->ZC_wn_Variable.at({w, n}) - 3*this->ZT_uvwn_Variable.at({u, v, w, n}) <= 0);
//                    this->solver.addConstraint(this->ZA_un_Variable.at({u, n}) + this->ZB_vn_Variable.at({v, n}) + this->ZC_wn_Variable.at({w, n}) - 3*this->ZT_uvwn_Variable.at({u, v, w, n}) >= -2);


                }
            }
        }
    }
    //setup the list that satisfy the equation
    SetPT_SumList();

    //set pt sum constraint
    SetPT_Sum();

    //set sum Optimizer constraint if turned on
    if (sumOpt){
        sumOptimizer();
    }
}

void ScalpHandler::SetPT_SumList(){
    for (int w = 0; w <= (x * z)-1; w++) { //initialize w
        for (int b = 0; b <= y-1; b++){
            int u,v;

            //Horizontal u
            u=b+((w/z)*y);

            //Vertical v
            v=(b*z)+(w%z);

            for (int n = 0; n <= N-1; n++){
                std::string pt="PT_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w) + "_" + std::to_string(n);
                PT_List[pt]=this->PT_uvwn_Variable[{u, v, w, n}];
//                std::cout << "PT_List is " << this->PT_List[pt] << std::endl;
            }
        }
    }
}

void ScalpHandler::SetPT_Sum() {
    std::string pt;
    for(int u = 0; u <= (x*y)-1; ++u) {
        for(int v = 0; v <= (y*z)-1; ++v) {
            for(int w = 0; w <= (x*z)-1; ++w) {
                PT_Sum[{u, v, w}]=0;
//               std::cout << "Start " << this->PT_Sum[{u, v, w}] << std::endl;
                for(int n = 0; n <= N-1; ++n) {
                    pt="PT_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w) + "_" + std::to_string(n);
                    PT_Sum[{u, v, w}] += this->PT_uvwn_Variable[{u, v, w, n}];
                }
                if (PT_List.count(pt)>0) {
//                    std::cout << "PT = " << pt << " exist in the map " << std::endl;
//                    std::cout << "End \n" << this->PT_Sum[{u, v, w}] << "=1" << std::endl;
                    this->solver.addConstraint(PT_Sum[{u, v, w}] == 1);
                }

                else{
//                    std::cout << "PT = " << pt << " doesnt exist in the map"  << std::endl;
//                    std::cout << "End \n" << this->PT_Sum[{u, v, w}] << "=0" << std::endl;
                    this->solver.addConstraint(PT_Sum[{u, v, w}] == 0);
                }
            }
        }
    }
}

void ScalpHandler::RunSolver() {
    std::cout<<"MatMul: Starting Solver"<<std::endl;
    status = this->solver.solve();
    std::cout << "\n\n\nMatMul: ScaLP finished solving with status " << ScaLP::showStatus(status) << std::endl;
    solverStatus =ScaLP::showStatus(status);
    if(status == ScaLP::status::FEASIBLE or status == ScaLP::status::OPTIMAL or status == ScaLP::status::TIMEOUT_FEASIBLE or status == ScaLP::status::SOLUTION_LIMIT_FEASIBLE or status == ScaLP::status::MEM_LIMIT_FEASIBLE) {
        solverFeasibility = "feasible";
        endTime = std::chrono::high_resolution_clock::now();
        solverDuration = endTime - startTime;
        result = solver.getResult();
        resultValues = result.values;
    }
    else{
        solverFeasibility = "Not Feasible";
        return;
    }
}

std::map<ScaLP::Variable, double> ScalpHandler::getResultValues() {
    return resultValues;
}

std::chrono::duration<double> ScalpHandler::getDuration() {
        return solverDuration;
}

std::map<std::tuple<int, int, int, int>, ScaLP::Variable> ScalpHandler::getPT(){
    return PT_uvwn_Variable;
}




void ScalpHandler::sumOptimizer() {
    for(int u = 0; u <= (x*y)-1; ++u) {
        for(int v = 0; v <= (y*z)-1; ++v) {
            for(int w = 0; w <= (x*z)-1; ++w) {
                for(int n = 0; n <= N-1; ++n) {
                    sumOptimizerTerm += this->ZT_uvwn_Variable[{u, v, w, n}];

                }
            }

        }
    }
    solver.setObjective(ScaLP::maximize(sumOptimizerTerm));
}

std::map<std::string, std::string> ScalpHandler::getSolverData() {
    totalNumberVariables += SA_un_Variable.size();
    totalNumberVariables += ZA_un_Variable.size();
    totalNumberVariables += SB_vn_Variable.size();
    totalNumberVariables += ZB_vn_Variable.size();
    totalNumberVariables += SC_wn_Variable.size();
    totalNumberVariables += ZC_wn_Variable.size();
    totalNumberVariables += ST_uvwn_Variable.size();
    totalNumberVariables += ZT_uvwn_Variable.size();
    totalNumberVariables += PT_uvwn_Variable.size();

    //Constraints in set_Constraints * all uvwn Variables.
    totalNumberConstraints = 16 *  PT_uvwn_Variable.size();
    if (sumOpt){
        totalNumberConstraints +=1;
    }
    //Total number of constaints in pt Sum constraint
    int ptSumConstraint = (x*y)*(y*z)*(x*z);
    totalNumberConstraints +=ptSumConstraint;

    solverData.insert({"totalVariables",std::to_string(totalNumberVariables)});
    solverData.insert({"totalConstraints",std::to_string(totalNumberConstraints)});
    solverData.insert({"feasibility",solverFeasibility});
    solverData.insert({"status",solverStatus});





    return solverData;
}

std::map<std::string, ScaLP::Variable> ScalpHandler::getPT_List() {
    return PT_List;
}

std::map<std::tuple<int, int>, ScaLP::Variable> ScalpHandler::getSA(){
    return SA_un_Variable;

}
std::map<std::tuple<int, int>, ScaLP::Variable> ScalpHandler::getSB(){
    return SB_vn_Variable;
}

std::map<std::tuple<int, int>, ScaLP::Variable> ScalpHandler::getSC(){
    return SC_wn_Variable;
}

std::map<std::tuple<int, int>, ScaLP::Variable> ScalpHandler::getZA() {
    return ZA_un_Variable;
}

std::map<std::tuple<int, int>, ScaLP::Variable> ScalpHandler::getZB() {
    return ZB_vn_Variable;
}

std::map<std::tuple<int, int>, ScaLP::Variable> ScalpHandler::getZC() {
    return ZC_wn_Variable;
}

bool ScalpHandler::solverStat() {
    if (solverFeasibility=="feasible"){
        return true;
    }else{
        return false;
    };
}

void ScalpHandler::setParamList(const std::map<std::string, std::string> & para) {
    solver.paramMap=para;
    paramList=para;
    if (!paramList.empty()) {
    std::cout<<"\nMatMul:Manual parameter is called\n";
        for(auto const & it:paramList){
        std::cout<<"MatMul:"<<it.first<<" is set to " << it.second<<std::endl;
        }
        std::cout<<"\n";
    }
}











