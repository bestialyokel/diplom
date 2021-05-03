
#include <array>
#include <iostream>
#include <eigen3/Eigen/Dense>

#include "types.h"
#include "machine.h"

using namespace std;
using namespace Eigen;


void Machine::init(double U_in, double I_out) {
    const int l = 2*N;

    MatrixXd A(l, l);
    VectorXd b(l);

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

    for (int i = 0; i < N; i++) {
        const int idx = i*2;
        y0[i] = x(idx);
        z0[i] = x(idx+1);
    }
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
    return (y - Iout)/ NC;
}

Payload Machine::processNextPayload() {
    Payload res;
    const int l = min(pl.I.size(), pl.U.size());

    res.I = vector<double>(l);
    res.U = vector<double>(l);

    double k1,k2,k3,k4,q1,q2,q3,q4;

    double h = pl.tau;
    for (int i = 0; i < l; i++) {
        //first I_L and U_C
        k1 = f(pl.U[i], y0[0], z0[0]);
        q1 = g(y0[1], y0[0], z0[0]);

        k2 = f(pl.U[i], y0[0] + k1/2.0, z0[0] + q1/2.0);
        q2 = g(y0[1], y0[0] + k1/2.0, z0[0] + q1/2.0);

        k3 = f(pl.U[i], y0[0] + k2/2.0, z0[0] + q2/2.0);
        q3 = g(y0[1], y0[0] + k2/2.0, z0[0] + q2/2.0);

        k4 = f(pl.U[i], y0[0] + k3, z0[0] + q3);
        q4 = g(y0[1], y0[0] + k3, z0[0] + q3);

        y0[0] = y0[0] + h*(k1 + 2*k2 + 2*k3 + k4)/6;
        z0[0] = z0[0] + h*(q1 + 2*q2 + 2*q3 + q4)/6;

        for (int j = 1; j < N-1; j++) {
            k1 = f(z0[j-1], y0[j], z0[j]);
            q1 = g(y0[j+1], y0[j], z0[j]);

            //+h/2?
            k2 = f(z0[j-1], y0[j] + k1/2.0, z0[j] + q1/2.0);
            q2 = g(y0[j+1], y0[j] + k1/2.0, z0[j] + q1/2.0);
            //+h/2?
            k3 = f(z0[j-1], y0[j] + k2/2.0, z0[j] + q2/2.0);
            q3 = g(y0[j+1], y0[j] + k2/2.0, z0[j] + q2/2.0);

            k4 = f(z0[j-1], y0[j] + k3, z0[j] + q3);
            q4 = g(y0[j+1], y0[j] + k3, z0[j] + q3);

            y0[j] = y0[j] + h*(k1 + 2*k2 + 2*k3 + k4)/6;
            z0[j] = z0[j] + h*(q1 + 2*q2 + 2*q3 + q4)/6;
        }
        //last idx
        const int lIdx = N-1;
        k1 = f(z0[lIdx-1], y0[lIdx], z0[lIdx]);
        q1 = g(pl.I[i], y0[lIdx], z0[lIdx]);

        k2 = f(z0[lIdx-1], y0[lIdx] + k1/2.0, z0[lIdx] + q1/2.0);
        q2 = g(pl.I[i], y0[lIdx] + k1/2.0, z0[lIdx] + q1/2.0);

        k3 = f(z0[lIdx-1], y0[lIdx] + k2/2.0, z0[lIdx] + q2/2.0);
        q3 = g(pl.I[i], y0[lIdx] + k2/2.0, z0[lIdx] + q2/2.0);

        k4 = f(z0[lIdx-1], y0[lIdx] + k3, z0[lIdx] + q3);
        q4 = g(pl.I[i], y0[lIdx] + k3, z0[lIdx] + q3);

        res.I[i] = y0[lIdx];
        res.U[i] = z0[lIdx];

        y0[lIdx] = y0[lIdx] + h*(k1 + 2*k2 + 2*k3 + k4)/6;
        z0[lIdx] = z0[lIdx] + h*(q1 + 2*q2 + 2*q3 + q4)/6;
    }

    return res;
}

Machine::Machine(int amount, const RLC& lOptions)
    : opt(lOptions),
      N(amount)
    {
    RN = lOptions.R/N;
    NL = lOptions.L/N;
    NC = lOptions.C/N;

    y0 = vector<double>(N);
    z0 = vector<double>(N);
}

