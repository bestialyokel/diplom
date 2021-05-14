#ifndef TYPES_H
#define TYPES_H

using namespace std;
#include <vector>
#include <string>

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

class Encoder {
    public:
        virtual vector<double> encode(const string& s) = 0;
};


class RZEncoder : Encoder {
    private:
        double uPos;
        double uNeg;
        size_t tactCount;
    public:
        vector<double> encode(const string& s) override {
            auto res = vector<double>();
            for (auto& c : s) {
                if (c == '0') {

                } else {

                }
            }
        };

        RZEncoder(double uPos, double uNeg, size_t tactCount) : uPos(uPos), uNeg(uNeg), tactCount(tactCount) {}
};


#endif // TYPES_H
