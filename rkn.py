import matplotlib.pyplot as plt

L = 250
R = 10
C = 1

N = 10

#seems legit

def UinFunc(t):
    if (t > 100):
        return 12.5
    return 12

def IinFunc(t):
    if (t > 500):
        return 0.5
    return 1

def f(Uin, y, z):
    return ( Uin - R * y - z ) / L

def g(Iin, y, z):
    return ( y - Iin ) / (C)


y0 = [0] * (N+1)
z0 = [0] * (N+1)

y1 = [0] * (N+1)
z1 = [0] * (N+1)


y0[0] = IinFunc(0)
z0[0] = UinFunc(0)


x0 = 0
X = 1000
h = 1


xArr = []
yArr = []
zArr = []

while (x0 < X):
    k1 = h * f(UinFunc(x0), y0[0], z0[0])
    q1 = h * g(IinFunc(x0), y0[0], z0[0])

    k2 = h * f(UinFunc(x0 + h/2), y0[0] + q1/2, z0[0] + k1/2)
    q2 = h * g(IinFunc(x0 + h/2), y0[0] + q1/2, z0[0] + k1/2)

    k3 = h * f(UinFunc(x0 + h/2), y0[0] + q2/2, z0[0] + k2/2)
    q3 = h * g(IinFunc(x0 + h/2), y0[0] + q2/2, z0[0] + k2/2)

    k4 = h * f(UinFunc(x0 + h), y0[0] + q3, z0[0] + k3)
    q4 = h * g(IinFunc(x0 + h), y0[0] + q3, z0[0] + k3)

    y1[0] = y0[0]
    z1[0] = z0[0]

    y0[0] = y0[0] + (k1 + 2*k2 + 2*k3 + k4)/6
    z0[0] = z0[0] + (q1 + 2*q2 + 2*q3 + q4)/6

    #y0 - падение силы тока?
    #z0 - падение напряжения?

    for i in range(N-1):
        prevY = y1[0]
        prevZ = z1[0]
        k1 = h * f(prevY, y0[i+1], z0[i+1])
        q1 = h * g(prevZ, y0[i+1], z0[i+1])

        k2 = h * f(prevY + h/2, y0[i+1] + q1/2, z0[i+1] + k1/2)
        q2 = h * g(prevZ + h/2, y0[i+1] + q1/2, z0[i+1] + k1/2)

        k3 = h * f(prevY + h/2, y0[i+1] + q2/2, z0[i+1] + k2/2)
        q3 = h * g(prevZ + h/2, y0[i+1] + q2/2, z0[i+1] + k2/2)

        k4 = h * f(prevY + h, y0[i+1] + q3, z0[i+1] + k3)
        q4 = h * g(prevZ + h, y0[i+1] + q3, z0[i+1] + k3)

        y1[i+1] = y0[i+1]
        z1[i+1] = z0[i+1]

        y0[i+1] = y0[i+1] + (k1 + 2*k2 + 2*k3 + k4)/6
        z0[i+1] = z0[i+1] + (q1 + 2*q2 + 2*q3 + q4)/6


    xArr.append(x0)
    yArr.append(y0[N-1])
    zArr.append(z0[N-1])

    x0 += h


plt.plot(xArr, yArr, 'r')
plt.plot(xArr, zArr, 'g')
plt.show()
#
#
#
