#ifndef MACHINE_H
#define MACHINE_H

#include <array>
#include <iostream>
#include <eigen3/Eigen/Dense>
#include "types.h"

#include <thread>
#include <stop_token>
#include <optional>
#include <atomic>

using namespace std;
using namespace Eigen;

class Machine {
    typedef vector<double> state_type;
    RLC opt;
    int N;

    double RN;
    double NL;
    double NC;

    Payload pl;


    auto initState(double U_in, double I_out);
    auto f(double Uin, double y, double z);
    auto g(double Iout, double y, double z);

    auto iter(const state_type &x, state_type &dxdt, double t);

    public:

        void appendPayload(const Payload& p);

        Payload processNextPayload();

        optional<Payload> processNextPayloadStoppable(reference_wrapper<atomic_bool> shouldStop);

        Machine(int amount, const RLC& lOptions);
        ~Machine();
};

#endif // MACHINE_H
