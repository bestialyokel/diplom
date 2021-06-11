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
#include "types.h"

using namespace std;
using namespace Eigen;

class Machine {
    typedef vector<double> state_type;
    RLC opt;
    int N;

    double RN;
    double NL;
    double NC;

    auto initState(double U_in, double I_out);

    public:
        optional<Payload> processNextPayloadStoppable(Payload& payload, reference_wrapper<atomic_bool> shouldStop, function<void(int)> cb);

        Machine(int amount, const RLC& lOptions);
};

#endif // MACHINE_H
