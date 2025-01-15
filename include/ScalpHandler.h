//
// Created by lunug on 10/14/23.
//

#ifndef MATRIXMULT_SCALPHANDLER_H
#define MATRIXMULT_SCALPHANDLER_H

#include <string>


class s_init{
public:
    static void init(const std::string& ScalpSolver);
};

class s_var{
private:
    int private_a;
    int private_b;
public:
    void setint(int a);
    void setbinary(bool b);
};


#endif //MATRIXMULT_SCALPHANDLER_H
