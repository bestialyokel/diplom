#include "encoder.h"

vector<double> RZEncoder::encode(const string &s) {
    auto res = vector<double>();
    for (auto c : s) {
        double val = c == '0' ? uPos : uNeg;

        for (int i = 0; i < halfCnt; i++) {
            res.push_back(val);
        }

        for (int i = 0;  i < halfCnt; i++) {
            res.push_back(zero);
        }
    }
    return res;
};
