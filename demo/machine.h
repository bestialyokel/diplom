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

    public:
        vector<double> y0;
        vector<double> z0;

        void init(double U_in, double I_out);

        void appendPayload(const Payload& p);

        auto f(double Uin, double y, double z);

        auto g(double Iout, double y, double z);

        Payload processNextPayload();

    public:
    Machine(int amount, const RLC& lOptions);
};

#endif // MACHINE_H
