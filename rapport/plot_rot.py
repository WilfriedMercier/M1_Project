import numpy as np
import matplotlib.pyplot as plt
import matplotlib

PI = 4*np.arctan(1.)

matplotlib.rcParams.update({'font.size': 14})

latt, vit = np.loadtxt("liste_points", unpack=True, skiprows=1)

rayon = 0.6957e6

#vit *= (rayon/3600)*PI/180

vit *= (PI/180)*(rayon/3600)*(np.cos(latt*PI/180))
print(np.abs(latt))

plt.plot(latt, vit, "o")
plt.xlabel("Lattitude (°)")
plt.ylabel("Vitesse de rotation (km/s)")

plt.legend()

plt.savefig("images/vit_rot.png")
plt.show()


