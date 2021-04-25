import matplotlib.pyplot as plt

#y == I(L), z == U(C), x ~ t
#I(L)' = (U0 - RI(L) - U(C))/L
#U(C)' = I(L)/C

L = 25
R = 1
C = 0.1
N = 10

U0 = 2.5
I0 = 0.04

def yD(x, y, z):
    return (U0 - R*y - z)/L

def zD(x, y, z):
    return y/C



X = 10

x0 = 0
h = 0.1

y0 = [0] * N
y1 = [0] * N
z0 = [0] * N
z1 = [0] * N

y0[0] = I0
z0[0] = U0

xArr = []
lastY = []
lastZ = []

was = False

while (x0 < X):
    k1 = h * yD(x0, y0[0], z0[0])
    q1 = h * zD(x0, y0[0], z0[0])

    k2 = h * yD(x0 + h/2, y0[0] + q1/2, z0[0] + k1/2)
    q2 = h * zD(x0 + h/2, y0[0] + q1/2, z0[0] + k1/2)

    k3 = h * yD(x0 + h/2, y0[0] + q2/2, z0[0] + k2/2)
    q3 = h * zD(x0 + h/2, y0[0] + q2/2, z0[0] + k2/2)

    k4 = h * yD(x0 + h, y0[0] + q3, z0[0] + k3)
    q4 = h * zD(x0 + h, y0[0] + q3, z0[0] + k3)

    y1[0] = y0[0] + (k1 + 2*k2 + 2*k3 + k4)/6
    z1[0] = z0[0] + (q1 + 2*q2 + 2*q3 + q4)/6

    y0[0] = y1[0]
    z0[0] = z1[0]
    for i in range(N-1):
        print(i)
        print(k1)
        k1 = h * yD(x0, y0[i], z0[i])
        q1 = h * zD(x0, y0[i], z0[i])

        k2 = h * yD(x0 + h/2, y0[i] + q1/2, z0[i] + k1/2)
        q2 = h * zD(x0 + h/2, y0[i] + q1/2, z0[i] + k1/2)

        k3 = h * yD(x0 + h/2, y0[i] + q2/2, z0[i] + k2/2)
        q3 = h * zD(x0 + h/2, y0[i] + q2/2, z0[i] + k2/2)

        k4 = h * yD(x0 + h, y0[i] + q3, z0[i] + k3)
        q4 = h * zD(x0 + h, y0[i] + q3, z0[i] + k3)

        y1[i+1] = y0[i] + (k1 + 2*k2 + 2*k3 + k4)/5
        z1[i+1] = z0[i] + (q1 + 2*q2 + 2*q3 + q4)/5

        y0[i+1] = y1[i+1]
        z0[i+1] = z1[i+1]

    xArr.append(x0)
    lastY.append(y0[N-1])
    lastZ.append(z0[N-1])

    x0 = x0 + h


#y == I(L), z == U(C), x ~ t
#y' = (U0 - Ry - z)/L
#z' = y/C


plt.plot(xArr, lastY, 'r')
plt.plot(xArr, lastZ, 'g')
plt.show()

#print(xArr,yArr, zArr)

