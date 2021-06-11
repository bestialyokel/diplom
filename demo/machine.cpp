
#include <array>
#include <iostream>
#include <eigen3/Eigen/Dense>

#include "types.h"
#include "machine.h"

#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>
#include <functional>


using namespace std;
using namespace Eigen;
using namespace boost::numeric::odeint;

auto Machine::initState(double U_in, double I_out) {
    const int l = 2*N;

    MatrixXd A = MatrixXd::Zero(l, l);
    VectorXd b = VectorXd::Zero(l);

    vector<double> I = vector<double>(N);
    vector<double> U = vector<double>(N);

    b(0) = U_in;
    b(l - 1) = I_out;

    if (N == 1) {
        I[0] = I_out;
        U[0] = U_in - RN*I_out ;
        return make_pair(I, U);
    }

    A(0,0) = RN;
    A(0,1) = 1;
    A(1,0) = 1;

    if (N > 1) {
        A(1,2) = -1;

        A(l-2, l-3) = 1;
        A(l-2, l-2) = -RN;
        A(l-2, l-1) = -1;

        A(l-1, l-2) = 1;
    }

    for (int i = 1; i < N-1; i++) {
        const int idx = i*2;
        A(idx, idx-1) = 1;
        A(idx, idx) = -RN;
        A(idx, idx+1) = -1;
        A(idx+1, idx) = 1;
        A(idx+1, idx+2) = -1;
    }

    VectorXd x = A.householderQr().solve(b);

    for (int i = 0; i < N; i++) {
        const int idx = i*2;
        I[i] = x(idx);
        U[i] = x(idx+1);
    }
    return make_pair(I,U);
}


optional<Payload> Machine::processNextPayloadStoppable(Payload& payload, reference_wrapper<atomic_bool> shouldStop, function<void(int)> cb) {
    Payload res;
    res.tau = payload.tau;

    auto [I0, U0] = initState(payload.U[0], payload.I[0]);

    state_type state = vector<double>(2*N);
    boost::numeric::odeint::runge_kutta4_classic<state_type> rk;

    for (int i = 0; i < N; i++) {
        const int idx = i*2;
        state[idx] = I0[i];
        state[idx+1] = U0[i];
    }

    auto& U_in = payload.U;
    auto& I_out = payload.I;
    double h = payload.tau;

    size_t l = min(U_in.size(), I_out.size());

    vector<double> I_last = vector<double>(l);
    vector<double> U_last = vector<double>(l);

    size_t i = 0;
    const int lIdx = N*2 - 1;
    int percent = 1;
    do {
        atomic_bool& stop = shouldStop.get();

        if (stop.load()) {
            return nullopt;
        }

        double progress = (i / (double) l) * 100;
        int p = (int) progress;
        if (p != percent - 1) {
            cb(progress);
            percent = p;
        }

        const double t = i*h;
        I_last[i] = state[lIdx-1];
        U_last[i] = state[lIdx];

        rk.do_step(
            [&](const state_type &x, state_type &dxdt, double t) {

                double time = (t/h);

                size_t i = (size_t)time;

                if (N == 1) {
                    dxdt[0] = (U_in[i] - RN*x[0] - x[1])/NL;
                    dxdt[1] = (x[0] - I_out[i])/NC;
                    return;
                }

                dxdt[0] = (U_in[i] - RN*x[0] - x[1])/NL;
                dxdt[1] = (x[0] - x[2])/NC;

                for (int i = 1; i < N-1; i++) {
                    const int idx = i*2;
                    dxdt[idx] = (x[idx - 1] - RN*x[idx] - x[idx+1])/NL;
                    dxdt[idx+1] = (x[idx] - x[idx+2])/NC;
                }

                const int lIdx = N*2 - 1;
                dxdt[lIdx - 1] = (x[lIdx - 2] - RN*x[lIdx - 1] - x[lIdx])/NL;
                dxdt[lIdx] = (x[lIdx - 1] - I_out[i])/NC;

            },
            state,
            t,
            h
        );
        i++;
    }  while(i < l);

    res.I = I_last;
    res.U = U_last;
    return res;
}

Machine::Machine(int amount, const RLC& lOptions)
    : opt(lOptions),
    N(amount)
    {
        RN = lOptions.R/N;
        NL = lOptions.L/N;
        NC = lOptions.C/N;
    }

