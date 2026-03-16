import numpy as np
import matplotlib.pyplot as plt

# =========================
# Euler framåt
# =========================
def euler_forward(a, f, x0, y0, x_end, h):
    N = int((x_end - x0) / h)
    x = np.zeros(N + 1)
    y = np.zeros(N + 1)

    x[0] = x0
    y[0] = y0

    for n in range(N):
        y[n+1] = y[n] + h * (a * y[n] + f(x[n]))
        x[n+1] = x[n] + h

    return x, y


# =========================
# Parametrar
# =========================
a = -2
x0 = 0
y0 = 0
x_end = 10

h_values = [10/8, 1, 10/12, 0.02]
f = lambda x: np.sin(x)

# =========================
# Plottning
# =========================
plt.figure(figsize=(8, 5))

for h in h_values:
    x, y = euler_forward(a, f, x0, y0, x_end, h)
    plt.plot(x, y, label=f"h = {h}")

plt.xlabel("x")
plt.ylabel("y")
plt.title(r"Lösning av $y' = ay + \sin x$ med Euler framåt")
plt.legend()
plt.ylim(-0.5,0.5)
plt.grid(True)
plt.show()
