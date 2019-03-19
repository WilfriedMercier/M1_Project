import matplotlib
import matplotlib.pyplot as plt
import numpy as np

PI = 4*np.arctan(1.)

matplotlib.rcParams.update({'font.size': 14})

nom = "rotation"

ordonne_origine = -160171.531250
pente = 0.610477


#premier histo

x, y = np.loadtxt(nom + ".dat", unpack=True, skiprows=1)

x /= 3600
y *= 180/PI

plt.plot(x,y, 'or')

x1 = np.arange(x[0], x[np.size(x)-1], 10)
y1 = ordonne_origine + pente*x1[:]
plt.plot(x1, y1, 'b')

plt.xlabel("Temps normalisé " + r'$t/t_0$')
plt.ylabel("Longitude (°)")

plt.savefig("images/" + nom + ".png")
plt.show()
