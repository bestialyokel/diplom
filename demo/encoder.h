#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include <string>

using namespace std;

class Encoder {
    public:
        virtual vector<double> encode(const string& s) = 0;
};


class RZEncoder : Encoder {
    private:
        double uPos;
        double uNeg;
        double zero;
        size_t halfCnt;
    public:
        vector<double> encode(const string& s) override;

        RZEncoder(double zero, double step, size_t halfCnt) : uPos(zero+step), uNeg(zero-step), zero(zero), halfCnt(halfCnt) {}
};


#endif // ENCODER_H
