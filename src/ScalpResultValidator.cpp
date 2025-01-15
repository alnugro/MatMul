//
// Created by anug on 11/14/23.
//

#include "ScalpResultValidator.h"





ScalpResultValidator::ScalpResultValidator(const std::map<ScaLP::Variable, double> &res,
                                           const std::map<std::tuple<int, int, int, int>, ScaLP::Variable> &PT_uvwn,
                                           const std::map<std::string, ScaLP::Variable> &PT_List):resultValues(res),PT_List(PT_List),PT_uvwn_Variable(PT_uvwn) {


}

void ScalpResultValidator::setValidatorArg(const std::map<std::string, std::string> &arg) {
        x = AdditionalFunc::StringToInt(arg.at("x"));
        y = AdditionalFunc::StringToInt(arg.at("y"));
        z = AdditionalFunc::StringToInt(arg.at("z"));
        N = AdditionalFunc::StringToInt(arg.at("N"));
}

void ScalpResultValidator::validate() {
    for (int u = 0; u <= (x * y) - 1; ++u) {
        for (int v = 0; v <= (y * z) - 1; ++v) {
            for (int w = 0; w <= (x * z) - 1; ++w) {
                std::string pt;
                for (int n = 0; n <= N - 1; ++n) {
                    tensorResult[{u, v, w}] +=resultValues.at(ScalpResultValidator::PT_uvwn_Variable.at({u, v, w, n}));
                    pt="PT_" + std::to_string(u) + "_" + std::to_string(v) + "_" + std::to_string(w) + "_" + std::to_string(n);
                }
                if (tensorResult[{u,v,w}]==1) {
                    if (PT_List.count(pt)>0) {
                        validatorResult = true;
                    }
                    else {
                        validatorResult = false;
                        throw std::out_of_range ("Exception Scalp Output Validator: Value is wrong");
                    }

                } else {
                    if (PT_List.count(pt)>0) {
                        validatorResult = false;
                        throw std::out_of_range ("Exception Scalp Output Validator: Value is wrong");

                    }
                    else {
                        validatorResult = true;
                    }

                }
            }
        }

    }
    if (validatorResult){
        std::cout << "MatMul: output is validated as correct"<<std::endl;
    }
    else{
        std::cout << "MatMul: output is validated as wrong"<<std::endl;
    }


}

std::map<std::tuple<int, int, int>, double> ScalpResultValidator::getTensorResult() {
    return tensorResult;
}

