import math

def f1(x):
    return x

def f2(x):
    return math.exp(-x)


def rektangelmetoden(f, a, b, n):
    h = (b - a) / n
    total = 0.0

    for i in range(n):
        x = a + i * h
        total += f(x)
    
    return h * total

def trapetsmetoden(f, a, b, n):
    h = (b - a) / n

    total = 0.5 * (f(a) + f(b))

    for i in range(1, n):
        x = a + i * h
        total += f(x)

    return h * total

def find_n(method, method_name, f, a, b, exact, tol=1e-6):
    print(method_name)
    
    n = 1
    
    while True:
        h = (b - a) / n
        approx = method(f, a, b, n)
        error = abs(approx - exact)
        
        print(f"h: {h:.3e}: int = {approx:.12f}, err = {error:.2e}")
        
        if error < tol:
            print(f"N = {n}\n")
            return n
        
        n *= 2

def main():
    exact1 = 0.5
    exact2 = 1 - math.exp(-1)

    # f(x) = x
    find_n(rektangelmetoden, "Rektangel f(x)=x", f1, 0, 1, exact1)
    find_n(trapetsmetoden, "Trapets f(x)=x", f1, 0, 1, exact1)

    # f(x) = e^(-x^2)
    find_n(rektangelmetoden, "Rektangel f(x)=e^(-x)", f2, 0, 1, exact2)
    find_n(trapetsmetoden, "Trapets f(x)=e^(-x)", f2, 0, 1, exact2)


if __name__ == "__main__":
    main()