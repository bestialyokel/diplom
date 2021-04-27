import matplotlib.pyplot as plt
import math

L = 25
R = 1
C = 0.1

N = 10

def f(Uin, y, z):
    return ( Uin - R / N * y - z ) / (N/L)

def g(Iin, y, z):
    return ( y - Iin ) / (N/C)

x0 = 0
X = 1000
h = 1

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


y0 = 0
z0 = 0
 
for i in range(1):
    yIn = Ivalues[0]
    y0 = Ivalues[0]
    z0 = Uvalues[0]
    y1 = z1 = 0
    for j in range(xCount):
        k1 = h * f(Uvalues[j], y0, z0)
        q1 = h * g(Ivalues[j], y0, z0)

        k2 = h * f(Uvalues[j], y0 + q1/2, z0 + k1/2)
        q2 = h * g(Ivalues[j], y0 + q1/2, z0 + k1/2)

        k3 = h * f(Uvalues[j], y0 + q2/2, z0 + k2/2)
        q3 = h * g(Ivalues[j], y0 + q2/2, z0 + k2/2)

        k4 = h * f(Uvalues[j], y0 + q3, z0 + k3)
        q4 = h * g(Ivalues[j], y0 + q3, z0 + k3)

        y1 = y0
        z1 = z0

        y0 = y0 + (k1 + 2*k2 + 2*k3 + k4)/6
        z0 = z0 + (q1 + 2*q2 + 2*q3 + q4)/6

        Ivalues[j] = y1 + (y1 - yIn)/(N/C)
        Uvalues[j] = z1
    
xArr = [0] * xCount
for i in range(xCount):
    xArr[i] = i*h


#plt.plot(xArr, Ivalues, 'r')
plt.plot(xArr, Uvalues, 'g')
plt.show()
#
#
#
