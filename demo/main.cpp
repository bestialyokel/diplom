#include "mainwindow.h"

#include <QApplication>
#include <array>
#include <iostream>

#include <eigen3/Eigen/Dense>

#include <atomic>

using namespace std;
using namespace Eigen;

typedef struct {
   double R;
   double L;
   double C;
} RLC;


typedef struct {
    double tau;
    vector<double> I;
    vector<double> U;
} Payload;


class Machine {
    RLC opt;
    int N;

    double RN;
    double NL;
    double NC;

    vector<double> y0;
    vector<double> z0;
    Payload p;

    public:
        void init(const Payload p) {
            const int l = 2*N;
            const double R0 = opt.R/N;

            MatrixXd A(l, l);
            VectorXd b(l);

            b(0) = p.U[0];
            b(l - 1) = p.I[0];

            A(0,0) = R0;
            A(0,1) = 1;
            A(1,0) = 1;

            if (N > 1) {
                A(1,2) = -1;
                A(l-2, l-3) = 1;
                A(l-2, l-2) = -R0;

                A(l-1, l-2) = 1;
            }

            for (int i = 1; i < N-1; i++) {
                const int idx = i*2;
                A(idx, idx-1) = 1;
                A(idx, idx) = -R0;
                A(idx, idx+1) = -1;
                A(idx+1, idx) = 1;
                A(idx+1, idx+2) = -1;
            }

        }

    public:

    auto f(double Uin, double y, double z) {
        return (Uin - (RN) * y - z) / NL;
    }

    auto g(double Iout, double y, double z) {
        return (y - Iout) / NC;
    }


    public:
    /*
        optional<Payload> ProcessPayloadNext() {
            Payload result;
            if (payloadList.size() == 0) {
                return nullopt;
            }
            Payload proc = payloadList[0];

            int length = min(proc.I.size(), proc.U.size());

            result.tau = proc.tau;
            result.I.reserve(length);
            result.U.reserve(length);

            //solve linear system when I_L' = 0 && U_C' = 0

            double k1,k2,k3,k4,q1,q2,q3,q4;


            for (int i = 0; i < N; i++) {
                result.I[i] = I0[i];
                result.U[i] = U0[i];
            }


            //time* cycle
            double h = proc.tau;
            for (int i = 0; i < length; i++) {
                //first I_L and U_C
                k1 = f(proc.U[i], I0[0], U0[0]);
                q1 = g(I0[1], I0[0], U0[0]);

                k2 = f(proc.U[i], I0[0] + k1/2.0, U0[0] + q1/2.0);
                q2 = g(I0[1], I0[0] + k1/2.0, U0[0] + q1/2.0);

                k3 = f(proc.U[i], I0[0] + k2/2.0, U0[0] + q2/2.0);
                q3 = g(I0[1], I0[0] + k2/2.0, U0[0] + q2/2.0);

                k4 = f(proc.U[i], I0[0] + k3, U0[0] + q3);
                q4 = g(I0[1], I0[0] + k3, U0[0] + q3);

                I0[0] = I0[0] + h*(k1 + 2*k2 + 2*k3 + k4)/6;
                U0[0] = U0[0] + h*(q1 + 2*q2 + 2*q3 + q4)/6;

                for (int j = 1; j < N-1; j++) {
                    k1 = f(U0[j-1], I0[j], U0[j]);
                    q1 = g(I0[j-1], I0[j], U0[j]);

                    k2 = f(U0[j-1], I0[j] + k1/2.0, U0[j] + q1/2.0);
                    q2 = g(I0[j-1], I0[j] + k1/2.0, U0[j] + q1/2.0);

                    k3 = f(U0[j-1], I0[j] + k2/2.0, U0[j] + q2/2.0);
                    q3 = g(I0[j-1], I0[j] + k2/2.0, U0[j] + q2/2.0);

                    k4 = f(U0[j-1], I0[j] + k3, U0[j] + q3);
                    q4 = g(I0[j-1], I0[j] + k3, U0[j] + q3);

                    I0[j] = I0[j] + h*(k1 + 2*k2 + 2*k3 + k4)/6;
                    U0[j] = U0[j] + h*(q1 + 2*q2 + 2*q3 + q4)/6;
                }
                //last idx
                const int lIdx = N-1;
                k1 = f(U0[lIdx-1], I0[lIdx], U0[lIdx]);
                q1 = g(proc.I[i], I0[lIdx], U0[lIdx]);

                k2 = f(U0[lIdx-1], I0[lIdx] + k1/2.0, U0[lIdx] + q1/2.0);
                q2 = g(proc.I[i], I0[lIdx] + k1/2.0, U0[lIdx] + q1/2.0);

                k3 = f(U0[lIdx-1], I0[lIdx] + k2/2.0, U0[lIdx] + q2/2.0);
                q3 = g(proc.I[i], I0[lIdx] + k2/2.0, U0[lIdx] + q2/2.0);

                k4 = f(U0[lIdx-1], I0[lIdx] + k3, U0[lIdx] + q3);
                q4 = g(proc.I[i], I0[lIdx] + k3, U0[lIdx] + q3);

                result.I[i] = I0[lIdx];
                result.U[i] = U0[lIdx];

                I0[lIdx] = I0[lIdx] + h*(k1 + 2*k2 + 2*k3 + k4)/6;
                U0[lIdx] = U0[lIdx] + h*(q1 + 2*q2 + 2*q3 + q4)/6;
            }

            return result;
        }

    */

    public:
    Machine(int amount, const RLC& lOptions)
        : N(amount),
          opt(lOptions)
        {
        RN = lOptions.R/N;
        NL = N/lOptions.L;
        NC = N/lOptions.C;

        y0.reserve(N);
        z0.reserve(N);


    }
};

int main()
{
    double tau = 1;
    double T = 1000;

    int tCount = (T/tau);

    Payload p;
    RLC rlc = {
        .R = 10,
        .L = 250,
        .C = 1,
    };

    p.tau = tau;

    p.I.reserve(tCount);
    p.U.reserve(tCount);

    for (int i = 0; i < tCount; i++) {
        const double t = i*tau;
        if (t > 100) {
            p.U[i] = 12.5;
        } else {
            p.U[i] = 12;
        }

        if (t > 500) {
            p.I[i] = 0.5;
        } else {
            p.I[i] = 1;
        }
    }

    Machine m(10, rlc);

    //m.append(std::move(p));

    //m.init(p);

    //Payload last = m.ProcessPayloadNext().value();

    for (int i = 0; i < tCount; i++) {
        //cout << last.U[i];
    }

    return 0;
}
