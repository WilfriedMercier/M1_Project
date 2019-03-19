import matplotlib
import matplotlib.pyplot as plt
import numpy as np

matplotlib.rcParams.update({'font.size': 14})


nom = "histogramme"


#premier histo

x, y = np.loadtxt(nom + ".dat", unpack=True)

y /= 1000

n_bin = np.size(x)

plt.hist(x, weights=y, bins=n_bin)
plt.plot([1000,1000],[0,450], color='red', linewidth=3)

plt.xlabel("Couleur des pixels")
plt.ylabel("Nombre d'occurences" + "(" + r"$\times 10^3$" + ")")

#second histo
"""
x, y = np.loadtxt(nom + "_corr.dat", unpack=True)

n_bin = np.size(x)

plt.hist(x, weights=y, bins=n_bin)
plt.plot([0,12000],[40000,40000], color='red')
plt.plot([7500,7500],[0,60000], color='red')
plt.plot([9100,9100],[0,60000], color='red')

plt.text(2000, 41000, r"$f_{max}/n$", fontsize=17)
plt.text(6500, 50000, r"$c_{min}$", fontsize=17)
plt.text(9300, 50000, r"$c_{max}$", fontsize=17)

plt.xlabel("Couleur des pixels")
plt.ylabel("Nombre d'occurences")
"""

plt.savefig("images/" + nom + ".png")
plt.show()
