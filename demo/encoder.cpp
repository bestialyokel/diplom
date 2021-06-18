#include "encoder.h"

Encoder::~Encoder() {

};

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

vector<double> MLT3Encoder::encode(const string &s) {
    auto res = vector<double>();
    bool isInc = true;
    double val = zero;
    for (auto c : s) {
        if (c != '0') {
            if (isInc) {
                if (val == uPos) {
                    val = zero;
                    isInc = false;
                } else if (val == zero) {
                    val = uPos;
                } else {
                    val = zero;
                }
            } else {
                if (val == uNeg) {
                    val = zero;
                    isInc = true;
                } else if (val == zero) {
                    val = uNeg;
                } else {
                    val = zero;
                }
            }
        }
        for (int i = 0; i < tactCnt; i++) {
            res.push_back(val);
        }
    }
    return res;
};

vector<double> ManchesterEncoder::encode(const string &s) {
    auto res = vector<double>();
    double fVal;
    double sVal;
    for (auto c : s) {
        if (c == '0') {
            fVal = uPos;
            sVal = zero;
        } else {
            fVal = zero;
            sVal = uPos;
        }

        for (int i = 0; i < halfCnt; i++) {
            res.push_back(fVal);
        }

        for (int i = 0;  i < halfCnt; i++) {
            res.push_back(sVal);
        }
    }
    return res;
};
