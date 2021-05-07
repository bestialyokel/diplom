#ifndef MACHINE_H
#define MACHINE_H

#include <array>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include "types.h"


using namespace std;
using namespace Eigen;

class Machine {
    RLC opt;
    int N;

    double RN;
    double NL;
    double NC;

    Payload pl;


    auto initState(double U_in, double I_out);
    auto f(double Uin, double y, double z);
    auto g(double Iout, double y, double z);
    auto iterRK(double h, double Uin, double Iout, double U0, double I0);

    public:

        void appendPayload(const Payload& p);

        Payload processNextPayload();

        Machine(int amount, const RLC& lOptions);
        ~Machine();
};

#endif // MACHINE_H
