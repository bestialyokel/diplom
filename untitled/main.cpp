#include "mainwindow.h"

#include <QApplication>
#include <array>

using namespace std;

typedef struct {
   float R;
   float L;
   float C;
} RLC;


typedef struct {
    float T;
    float tau;
} TimeOptions;

template<int N>
class Machine {
    RLC lineOptions;
    TimeOptions timeOptions;

    array<float, N> I_L;
    array<float, N> U_C;

    vector<float> U0;

    vector<float> I0;

    public:
    Machine(const RLC& lineOptions, const TimeOptions& timeOptions)
        : lineOptions(lineOptions),
          timeOptions(timeOptions) {}

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
