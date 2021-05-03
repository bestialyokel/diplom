import numpy as np

L = 250
R = 10
C = 1

I0 = 1
U0 = 12

N = 10

L0 = L/N
R0 = R/N
C0 = C/N

A = np.zeros(shape=(N*2,N*2))

b = np.zeros(N*2)

b[0] = U0
b[2*N-1] = I0

A[0][0] = R0
A[0][1] = 1
A[1][0] = 1
if (N > 1):
    A[1][2] = -1

    A[2*N - 2][2*N - 3] = 1
    A[2*N - 2][2*N - 2] = -R0
    A[2*N - 2][2*N - 1] = -1

    A[2*N - 1][2*N - 2] = 1


for i in range(1, N-1):
    idx = i*2
    A[idx][idx - 1] = 1
    A[idx][idx] = -R0
    A[idx][idx + 1] = -1

    A[idx + 1][idx] = 1
    A[idx + 1][idx + 2] = -1

print(A)

x = np.linalg.solve(A,b)
print(x)
