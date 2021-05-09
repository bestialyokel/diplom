
#include <array>
#include <iostream>
#include <eigen3/Eigen/Dense>

#include "types.h"
#include "machine.h"

#include <boost/array.hpp>
#include <boost/numeric/odeint.hpp>


using namespace std;
using namespace Eigen;
using namespace boost::numeric::odeint;

auto Machine::initState(double U_in, double I_out) {
    const int l = 2*N;

    MatrixXd A = MatrixXd::Zero(l, l);
    VectorXd b = VectorXd::Zero(l);

    b(0) = U_in;
    b(l - 1) = I_out;

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

    vector<double> I = vector<double>(N);
    vector<double> U = vector<double>(N);

    for (int i = 0; i < N; i++) {
        const int idx = i*2;
        I[i] = x(idx);
        U[i] = x(idx+1);
    }
    return make_pair(I,U);
}

void Machine::appendPayload(const Payload& p) {
    pl.tau = p.tau;
    pl.I = p.I;
    pl.U = p.U;
}

Payload Machine::processNextPayload() {
    Payload res;

    res.tau = pl.tau;

    //initial state for U_in_[i](t=0) I_out_[i](t=0))
    auto [I0, U0] = initState(pl.U[0], pl.I[0]);

    //state[2n] - U_in_[i](t=0)
    //state[2n+1] - I_out_[i](t=0))
    state_type state = vector<double>(2*N);
    boost::numeric::odeint::runge_kutta4_classic<state_type> rk;

    //fill state
    for (int i = 0; i < N; i++) {
        const int idx = i*2;
        state[idx] = I0[i];
        state[idx+1] = U0[i];
    }

    auto& U_in = pl.U;
    auto& I_out = pl.I;
    double h = pl.tau;

    size_t l = min(U_in.size(), I_out.size());

    vector<double> I_last = vector<double>(l);
    vector<double> U_last = vector<double>(l);

    size_t i = 0;
    const int lIdx = N*2 - 1;
    do {
        const double t = i*h;

        I_last[i] = state[lIdx-1];
        U_last[i] = state[lIdx];
        rk.do_step(
            [&](const state_type &x, state_type &dxdt, double t) {

                double uin = t > 100 ? 12.5 : 12;
                double iout = t > 500 ? 0.5 : 1;

                dxdt[0] = (uin - RN*x[0] - x[1])/NL;
                dxdt[1] = (x[0] - x[2])/NC;

                for (int i = 1; i < N-1; i++) {
                    const int idx = i*2;
                    dxdt[idx] = (x[idx - 1] - RN*x[idx] - x[idx+1])/NL;
                    dxdt[idx+1] = (x[idx] - x[idx+2])/NC;
                }

                const int lIdx = N*2 - 1;
                dxdt[lIdx - 1] = (x[lIdx - 2] - RN*x[lIdx - 1] - x[lIdx])/NL;
                dxdt[lIdx] = (x[lIdx - 1] - iout)/NC;
                /*
                dxdt[0] = (U_in[i] - RN*x[0] - x[1] + 1)/NL;
                dxdt[1] = (x[0] - x[2])/NC;

                for (int i = 1; i < N-1; i++) {
                    const int idx = i*2;
                    dxdt[idx] = (x[idx - 1] - RN*x[idx] - x[idx+1])/NL;
                    dxdt[idx+1] = (x[idx] - x[idx+2])/NC;
                }

                const int lIdx = N*2 - 1;
                dxdt[lIdx - 1] = (x[lIdx - 2] - RN*x[lIdx - 1] - x[lIdx])/NL;
                dxdt[lIdx] = (x[lIdx - 1] - I_out[i])/NC;
                */
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

Machine::~Machine() {

}

Machine::Machine(int amount, const RLC& lOptions)
    : opt(lOptions),
    N(amount)
    {
        RN = lOptions.R/N;
        NL = lOptions.L/N;
        NC = lOptions.C/N;
    }

