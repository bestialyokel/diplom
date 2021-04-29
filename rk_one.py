import matplotlib.pyplot as plt

L = 250
R = 10
C = 1
N = 10

t0 = 0
T = 1000
tau = 1

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
    return ( Uin - (R) * y - z ) / (L)

def g(Iin, y, z):
    return ( y - Iin ) / (C)


y0 = 1
z0 = 2

tArr = []
u_result = []
i_result = []

y1 = z1 = 0

for i in range(tCount - 2):
    k1 = f(U0[i], y0, z0)
    q1 = g(I0[i], y0, z0)

    k2 = f(U0[i+1], y0 + k1/2, z0 + q1/2)
    q2 = g(I0[i+1], y0 + k1/2, z0 + q1/2)

    k3 = f(U0[i+1], y0 + k2/2, z0 + q2/2)
    q3 = g(I0[i+1], y0 + k2/2, z0 + q2/2)

    k4 = f(U0[i+2], y0 + k3, z0 + q3)
    q4 = g(I0[i+2], y0 + k3, z0 + q3)

    y1 = y0 + tau * (k1 + 2*k2 + 2*k3 + k4)/6
    z1 = z0 + tau * (q1 + 2*q2 + 2*q3 + q4)/6
    tArr.append(i*tau)
    i_result.append(y0)
    u_result.append(z0)
    y0 = y1
    z0 = z1
    


plt.plot(tArr, i_result, 'r')
plt.plot(tArr, u_result, 'g')
plt.show()
#
#
#
