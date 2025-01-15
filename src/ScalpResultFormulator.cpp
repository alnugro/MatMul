//
// Created by anug on 11/15/23.
//

#include "ScalpResultFormulator.h"






ScalpResultFormulator::ScalpResultFormulator(const std::map<ScaLP::Variable, double> &res,
                                             const std::map<std::tuple<int, int, int, int>, ScaLP::Variable> &PT_uvwn)
                                             : resultValues(res), PT_uvwn_Variable(PT_uvwn) {

}

void ScalpResultFormulator::setFormulatorArg(const std::map<std::string, std::string> &arg) {
        x = AdditionalFunc::StringToInt(arg.at("x"));
        y = AdditionalFunc::StringToInt(arg.at("y"));
        z = AdditionalFunc::StringToInt(arg.at("z"));
        N = AdditionalFunc::StringToInt(arg.at("N"));
}


void ScalpResultFormulator::setSABC(const std::map<std::tuple<int, int>, ScaLP::Variable> &SA_un_Variable,
                                    const std::map<std::tuple<int, int>, ScaLP::Variable> &SB_vn_Variable,
                                    const std::map<std::tuple<int, int>, ScaLP::Variable> &SC_wn_Variable){
    SA=SA_un_Variable;
    SB=SB_vn_Variable;
    SC=SC_wn_Variable;
}

void ScalpResultFormulator::setZABC(const std::map<std::tuple<int, int>, ScaLP::Variable> &ZA_un_Variable,
                                    const std::map<std::tuple<int, int>, ScaLP::Variable> &ZB_vn_Variable,
                                    const std::map<std::tuple<int, int>, ScaLP::Variable> &ZC_wn_Variable) {
    ZA =ZA_un_Variable;
    ZB =ZB_vn_Variable;
    ZC =ZC_wn_Variable;
}


void ScalpResultFormulator::formulateRank1Tensor() {
        for (int u = 0; u <= (x * y) - 1; ++u) {
            for (int v = 0; v <= (y * z) - 1; ++v) {
                for (int w = 0; w <= (x * z) - 1; ++w) {
                    for (int n = 0; n <= N - 1; ++n) {
                        setRank1tensor(u,v,w,n);

                    }
                }
            }
        }

        //test
//    for(auto it: valueRank1tensor_U){
//        std::cout << "U: "<< std::get<0>(it.first)<< "."<<std::get<1>(it.first) <<"  "<< it.second << std::endl;
//    }
//    for(auto it: valueRank1tensor_V){
//        std::cout  << "V: "<< std::get<0>(it.first)<< "."<<std::get<1>(it.first) <<"  "<< it.second << std::endl;
//    }
//    for(auto it: valueRank1tensor_W){
//        std::cout  << "W: "<< std::get<0>(it.first)<< "."<<std::get<1>(it.first) <<"  "<< it.second << std::endl;
//    }

    equationExtractor();
    sumCounter();




}

void ScalpResultFormulator::setRank1tensor(int & u, int &v, int &w, int &n) {

    try {
        double resZC = 0;
        double resZB = 0;
        double resZA = 0;

        if(std::round(resultValues.at(ZA.at({u,n}))) == 0){
            resZA = 1;
        }
        if(std::round(resultValues.at(ZB.at({v,n}))) == 0){
            resZB = 1;
        }
        if(std::round(resultValues.at(ZC.at({w,n}))) == 0){
            resZC = 1;
        }

        double resSC = 1;
        double resSB = 1;
        double resSA = 1;

        if(std::round(resultValues.at(SA.at({u,n}))) == 1){
             resSA = -1;
        }
        if(std::round(resultValues.at(SB.at({v,n}))) == 1){
             resSB = -1;
        }
        if(std::round(resultValues.at(SC.at({w,n}))) == 1){
             resSC = -1;
        }



        insertTensor_U(std::make_tuple(u,n),resZA*resSA);
        insertTensor_V(std::make_tuple(v,n),resZB*resSB);
        insertTensor_W(std::make_tuple(w,n),resZC*resSC);


    } catch (const std::out_of_range& e) {
        std::cout<< u << v  << w << n << std::endl;
        std::cerr << "Exception caught: " << e.what() << std::endl;
        exit(0);
    }


}


void ScalpResultFormulator::insertTensor_U(std::tuple<int, int>  tup, double PT) {
    auto result = valueRank1tensor_U.find(tup);
    if (result == valueRank1tensor_U.end()) {
        // Key doesn't exist, insert the pair
        valueRank1tensor_U.insert({tup, PT});
    }
}

void ScalpResultFormulator::insertTensor_V(std::tuple<int, int>  tup, double PT) {
    auto result = valueRank1tensor_V.find(tup);
    if (result == valueRank1tensor_V.end()) {
        // Key doesn't exist, insert the pair
        valueRank1tensor_V.insert({tup, PT});
    }
}

void ScalpResultFormulator::insertTensor_W(std::tuple<int, int>  tup, double PT) {
    auto result = valueRank1tensor_W.find(tup);
    if (result == valueRank1tensor_W.end()) {
        // Key doesn't exist, insert the pair
        valueRank1tensor_W.insert({tup, PT});
    }
}



void ScalpResultFormulator::equationExtractor() {
    //Sum counter for w and also formulation of M
    std::vector<int> vectorN = {};
    for(auto it: valueRank1tensor_W){
        std::string key = std::to_string(std::get<0>(it.first));
        auto checkKey = mapWforN.find(key);
        if (checkKey == mapWforN.end()) {
            // Key doesn't exist declare the key
            mapWforN[key]= vectorN;
        }
        //if the value is one then formulate M
        if (std::round(it.second) == 1 || std::round(it.second) == -1){
            mapWforN[key].push_back(std::get<1>(it.first));
        }
    }

}

void ScalpResultFormulator::sumCounter() {
    //sum counter in w
    totalNumberSum =0;
    for (int w = 0; w <= (x * z) - 1; ++w) {
        int sumInN =0;
        std::string key = std::to_string(w);
        for (int num: mapWforN[key]) {
            sumInN +=1;
        }
        if(sumInN > 0){
            totalNumberSum += sumInN - 1;
        }
    }

    //sum counter in u & v
    for (int n = 0; n <= N - 1; ++n) {
        //sum counter in u
        int sumInU=0;
        for (int u = 0; u <= (x * y) - 1; ++u) {
            if (valueRank1tensor_U.at({u,n})!=0){
                sumInU+=1;
            }
        }

        if(sumInU>0){
            totalNumberSum +=sumInU - 1;
        }

        //sum counter in v
        int sumInV=0;
        for (int v = 0; v <= (y * z) - 1; ++v) {
            if (valueRank1tensor_V.at({v,n})!=0){
                sumInV+=1;
            }
        }

        if(sumInV>0){
            totalNumberSum +=sumInV - 1;
        }
    }

}


std::map<std::tuple<int, int>, double> ScalpResultFormulator::getRank1_U() {
    return valueRank1tensor_U;
}

std::map<std::tuple<int, int>, double> ScalpResultFormulator::getRank1_W() {
    return valueRank1tensor_W;
}

std::map<std::tuple<int, int>, double> ScalpResultFormulator::getRank1_V() {
    return valueRank1tensor_V;
}

std::map<std::string, std::vector<int>> ScalpResultFormulator::getMapWforN() {
    return mapWforN;
}

int ScalpResultFormulator::getTotalNumberSum() const {
    return totalNumberSum;
}







