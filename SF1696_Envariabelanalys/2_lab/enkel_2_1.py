import numpy as np

# =========================
# Euler framåt för y' = a y
#a – parameter i differentialekvationen
#f – högerledets funktion (här sin(𝑥))
#x0 – startvärde för 𝑥
#y0 – begynnelsevärde
#x_end – slutpunkt
#h – steglängd
# =========================
def euler_forward(a, x0, y0, x_end, h):
    N = int((x_end - x0) / h)
    y = y0
    x = x0
    
    for _ in range(N):
        y = y + h * a * y
        x = x + h
        
    return y


# =========================
# Exakt lösning
# =========================
def exact_solution(a, x0, y0, x):
    return y0 * np.exp(a * (x - x0))


# =========================
# Konvergenstabell
# =========================
def convergence_table(a=1, x0=0, y0=1, x_end=1,
                      start_h=0.5, halvningar=15):
    
    exact = exact_solution(a, x0, y0, x_end)
    
    h = start_h
    prev_error = None
    
    for i in range(halvningar):
        y_num = euler_forward(a, x0, y0, x_end, h)
        error = abs(y_num - exact)
        
        if prev_error is not None:
            rate = np.log2(prev_error / error)
            print(f"h: {h:.6f}: y({x_end}) = {y_num:.12f}, "
                  f"err = {error:.2e}, rate={rate:.2f}")
        else:
            print(f"h: {h:.6f}: y({x_end}) = {y_num:.12f}, "
                  f"err = {error:.2e}")
        
        prev_error = error
        h = h / 2


# =========================
# Kör
# =========================
if __name__ == "__main__":
    convergence_table(a=1, x_end=1)
