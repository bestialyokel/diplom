
#include <array>
#include <iostream>
#include <eigen3/Eigen/Dense>

#include "types.h"
#include "machine.h"

using namespace std;
using namespace Eigen;

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

auto Machine::f(double Uin, double y, double z) {
    return (Uin - (RN) * y - z) / (NL);
}

auto Machine::g(double Iout, double y, double z) {
    return (y - Iout) / NC;
}

//returns pair <I0, U0>
auto Machine::iterRK(double h, double Uin, double Iout, double U0, double I0) {
    double k1,k2,k3,k4,q1,q2,q3,q4;

    k1 = f(Uin, I0, U0);
    q1 = g(Iout, I0, U0);

    k2 = f(Uin, I0 + k1/2.0, U0 + q1/2.0);
    q2 = g(Iout, I0 + k1/2.0, U0 + q1/2.0);

    k3 = f(Uin, I0 + k2/2.0, U0 + q2/2.0);
    q3 = g(Iout, I0 + k2/2.0, U0 + q2/2.0);

    k4 = f(Uin, I0 + k3, U0 + q3);
    q4 = g(Iout, I0 + k3, U0 + q3);

    I0 = I0 + h*(k1 + 2*k2 + 2*k3 + k4)/6;
    U0 = U0 + h*(q1 + 2*q2 + 2*q3 + q4)/6;

    return make_pair(I0, U0);
}


Payload Machine::processNextPayload() {
    Payload res;
    const int l = min(pl.I.size(), pl.U.size());

    auto [I0, U0] = initState(pl.U[0], pl.I[0]);

    /*

    vector<vector<double>> U = vector<vector<double>>(N+1);
    vector<vector<double>> I = vector<vector<double>>(N+1);

    U[0] = pl.U;
    for (int i = 0; i < N; i++) {
        U[i+1] = vector<double>(l);
        I[i] = vector<double>(l);

        U[i+1][0] = U0[i];
        I[i][0] = I0[i];
    }
    I[N] = pl.I;
    */

    vector<double> I_L, U_C;
    I_L = vector<double>(l);
    U_C = vector<double>(l);

    double h = pl.tau;

    for (int i = 0; i < l; i++) {
        auto [y0, z0] = iterRK(h, pl.U[i], I0[1], U0[0], I0[0]);
        U0[0] = z0;
        I0[0] = y0;
        for (int j = 1; j < N-1; j++) {
            auto [y0, z0] = iterRK(h, U0[j-1], I0[j+1], U0[j], I0[j]);
            U0[j] = z0;
            I0[j] = y0;
        }
        auto [y1, z1] = iterRK(h, U0[N-2], pl.I[i], U0[N-1], I0[N-1]);
        U0[N-1] = z1;
        I0[N-1] = y1;
        I_L[i] = y1;
        U_C[i] = z1;
    }

    res.I = I_L;
    res.U = U_C;

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

