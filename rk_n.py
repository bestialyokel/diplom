import matplotlib.pyplot as plt

L = 250
R = 10
C = 1
N = 10

t0 = 0
T = 1000
tau = 1

def f(Uin, y, z):
    return ( Uin - (R/N) * y - z ) / (N/L)

def g(Iout, y, z):
    return ( y - Iout ) / (N/C)
    

tCount = int(T/tau)

U0 = [0] * tCount
I0 = [0] * tCount


for i in range(tCount):
    if (i >= 100):
        U0[i] = 12.5
    else:
        U0[i] = 12
    if (i >= 500):
        I0[i] = 0.5
    else:
        I0[i] = 1





# y = I_L, z = U_C, x == time
#y' = (U_in - R * y - z)/L = f(x,y,z)
#z' = (y - I_in)/C = g(x,y,z)

def f(Uin, y, z):
    return ( Uin - (R/N) * y - z ) / (N/L)

def g(Iout, y, z):
    return ( y - Iout ) / (N/C)




tArr = []

for i in range(N):
    y0 = I0[i]
    z0 = U0[i]
    y1 = z1 = 0
    for j in range(tCount):
        k1 = f(U0[j], y0, z0)
        q1 = g(I0[j], y0, z0)

        k2 = f(U0[j], y0 + k1/2, z0 + q1/2)
        q2 = g(I0[j], y0 + k1/2, z0 + q1/2)

        k3 = f(U0[j], y0 + k2/2, z0 + q2/2)
        q3 = g(I0[j], y0 + k2/2, z0 + q2/2)

        k4 = f(U0[j], y0 + k3, z0 + q3)
        q4 = g(I0[j], y0 + k3, z0 + q3)

        y1 = y0 + tau * (k1 + 2*k2 + 2*k3 + k4)/6
        z1 = z0 + tau * (q1 + 2*q2 + 2*q3 + q4)/6

        I0[j] = y1
        U0[j] = z1

        y0 = y1
        z0 = z1




for i in range(tCount):
    tArr.append(i*tau)


plt.plot(tArr, U0, 'r')
plt.plot(tArr, I0, 'g')
plt.show()
#
#
#
