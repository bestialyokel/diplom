import matplotlib.pyplot as plt

L = 25
R = 1
C = 0.1

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
    return ( Uin - R*y - z ) / L

def g(Iin, y, z):
    return ( y - Iin ) / C

y0 = IinFunc(0)
z0 = UinFunc(0)

y1 = z1 = 0

x0 = 0
X = 1000
h = 1


xArr = []
yArr = []
zArr = []

while (x0 < X):
    k1 = h * f(UinFunc(x0), y0, z0)
    q1 = h * g(IinFunc(x0), y0, z0)

    k2 = h * f(UinFunc(x0 + h/2), y0 + q1/2, z0 + k1/2)
    q2 = h * g(IinFunc(x0 + h/2), y0 + q1/2, z0 + k1/2)

    k3 = h * f(UinFunc(x0 + h/2), y0 + q2/2, z0 + k2/2)
    q3 = h * g(IinFunc(x0 + h/2), y0 + q2/2, z0 + k2/2)

    k4 = h * f(UinFunc(x0 + h), y0 + q3, z0 + k3)
    q4 = h * g(IinFunc(x0 + h), y0 + q3, z0 + k3)

    y1 = y0 + (k1 + 2*k2 + 2*k3 + k4)/6
    z1 = z0 + (q1 + 2*q2 + 2*q3 + q4)/6

    xArr.append(x0)
    yArr.append(y0)
    zArr.append(z0)

    x0 += h
    y0 = y1
    z0 = z1


plt.plot(xArr, yArr, 'r')
plt.plot(xArr, zArr, 'g')
plt.show()
#
#
#
