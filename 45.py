import matplotlib.pyplot as plt

#y == I(L), z == U(C), x ~ t
#y' = (U0 - Ry - z)/L
#z' = y/C

L = 25
R = 1
C = 0.1

U0 = 2.5
I0 = 0.04

def yD(x, y, z):
    return (U0 - R*y - z)/L

def zD(x, y, z):
    return y/C

X = 10

x0 = 0
h = 0.1

xArr = []
yArr = []
zArr = []

y0 = I0
y1 = I0
z0 = U0
z1 = U0

was = False

while (x0 < X):
    k1 = h * yD(x0, y0, z0)
    q1 = h * zD(x0, y0, z0)

    k2 = h * yD(x0 + h/2, y0 + q1/2, z0 + k1/2)
    q2 = h * zD(x0 + h/2, y0 + q1/2, z0 + k1/2)

    k3 = h * yD(x0 + h/2, y0 + q2/2, z0 + k2/2)
    q3 = h * zD(x0 + h/2, y0 + q2/2, z0 + k2/2)

    k4 = h * yD(x0 + h, y0 + q3, z0 + k3)
    q4 = h * zD(x0 + h, y0 + q3, z0 + k3)

    xArr.append(x0)
    yArr.append(y0)
    zArr.append(z0)
    
    y1 = y0 + (k1 + 2*k2 + 2*k3 + k4)/6
    z1 = z0 + (q1 + 2*q2 + 2*q3 + q4)/6

    y0 = y1
    z0 = z1
    x0 = x0 + h


#y == I(L), z == U(C), x ~ t
#y' = (U0 - Ry - z)/L
#z' = y/C

plt.plot(xArr, yArr, 'r')
plt.plot(xArr, zArr, 'g')
plt.show()

#print(xArr,yArr, zArr)