#include "mainwindow.h"

#include <QApplication>
#include <array>

using namespace std;

typedef struct {
   double R;
   double L;
   double C;
} RLC;


typedef struct {
    double T;
    double tau;
} TimeOptions;


class Machine {
    int N;
    RLC lineOptions;
    TimeOptions timeOptions;

    //I_L && U_C
    vector<double> U0;
    vector<double> I0;



    public:
    Machine(int amount, const RLC& lineOptions, const TimeOptions& timeOptions)
        : lineOptions(lineOptions),
          timeOptions(timeOptions),
          N(amount)
        {
        I0.reserve(N);
        U0.reserve(N);
    }

        void append(vector<float>&& U,  vector<float>&& I) {
            U0 = U;
            I0 = I;
        }



        auto getOutput() {
            return make_tuple(U0, I0);
        }




};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
