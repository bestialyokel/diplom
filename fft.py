import scipy.signal 
import matplotlib.pyplot as plt
import numpy as np

x = np.linspace(0, 10, 256, endpoint=False)
y = np.cos(-x**2/6.0)
yre = signal.resample(y,20)
xre = np.linspace(0, 10, len(yre), endpoint=False)
plt.plot(x,y,'b', xre,yre,'or-')
plt.show()