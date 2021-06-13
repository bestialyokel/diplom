#include <eigen3/Eigen/Dense>

int main() {
    size_t N = 4;

    MatrixXd A = MatrixXd::zero(N, N);
    VectorXd b = vectorXd::zero(N);

    b(0) = 12;
    b(N-1) = 1;

    matrix << 10, 1, 0, 0 // U_IN
           << 1, 0, -1, 0 // 0
           << 0, 1, -1, -1 //0
           << 0, 0, 0, 1 // i_out

    double R = 10;

    matrix << R, 1, 0, 0, 0, 0 // UIN
           << 1, 0, -1, 0, 0, 0 // 0
           << 0, 1, -R, -1, 0, 0 // 0
           << 0, 0, 1, 0, -1, 0 // 0
           << 0, 0, 0, 1, -R, -1 // 0
           << 0, 0, 0, 0, 1, 0 // IOUT

    vector<double> I = vector<double>(4);
    vector<double> U = vector<double>(4);

    for (size_t i = 0; i < 4; i++) {
        I[i] = I_out;
    }

    10/

    U[0] = 12 - R * I_out; // 2
    U[1] = 11 - R * I_out; 

    for (size_t i = 0; i < 4; i++) {
        U[i] = 
    }
}