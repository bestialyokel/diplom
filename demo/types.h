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

#endif // TYPES_H
