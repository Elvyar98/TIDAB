import numpy as np
import matplotlib.pyplot as plt

x = np.linspace(2, 7, 1000)  # 1000 punkter mellan -3 och 3
y = x - 3*np.cos(3*x) - 2

plt.plot(x, y, label="f(x) = x - 3cos(3x) - 2")
plt.axhline(0, color='black', linewidth=0.5)  # x-axeln
plt.xlabel("x")
plt.ylabel("f(x)")
plt.title("Graf över f(x)")
plt.legend()
plt.grid(True)
plt.show()