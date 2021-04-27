import matplotlib.pyplot as plt
import math

L = 250
R = 10
C = 1

N = 10

def f(Uin, y, z):
    return ( Uin - R * y - z ) / (L)

def g(Iin, y, z):
    return ( y - Iin ) / (C)

#
#I_L' = (U_in - R * I_L - U_C) / L
#U_C' = (I_L - I_in)/C

# I_C = U_C' = (I_L - I_in)/C

# I_out = I_L + I_C = I_L + (I_L - I_in)C
# U_out = U_C



y0 = y1 = z0 = z1 = 0


#?

yIn = 1

y0 = 1
z0 = 12

x0 = 0
X = 1000
h = 1


xArr = []
yArr = []
zArr = []

xCount = math.ceil(X/h)

Uvalues = [0] * xCount
Ivalues = [0] * xCount

for i in range(xCount):
    if (i*h < 100):
        Uvalues[i] = 12
    else:
        Uvalues[i] = 12.5
    if (i*h < 500):
        Ivalues[i] = 1
    else:
        Ivalues[i] = 0.5

for i in range(xCount):
    k1 = h * f(Uvalues[i], y0, z0)
    q1 = h * g(Ivalues[i], y0, z0)

    k2 = h * f(Uvalues[i], y0 + q1/2, z0 + k1/2)
    q2 = h * g(Ivalues[i], y0 + q1/2, z0 + k1/2)

    k3 = h * f(Uvalues[i], y0 + q2/2, z0 + k2/2)
    q3 = h * g(Ivalues[i], y0 + q2/2, z0 + k2/2)

    k4 = h * f(Uvalues[i], y0 + q3, z0 + k3)
    q4 = h * g(Ivalues[i], y0 + q3, z0 + k3)

    y1 = y0
    z1 = z0

    y0 = y0 + (k1 + 2*k2 + 2*k3 + k4)/6
    z0 = z0 + (q1 + 2*q2 + 2*q3 + q4)/6

    xArr.append(i*h)
    yArr.append(y0 + (y0 - yIn)/C)
    zArr.append(z0)


plt.plot(xArr, yArr, 'r')
plt.plot(xArr, zArr, 'g')
plt.show()
#
#
#
