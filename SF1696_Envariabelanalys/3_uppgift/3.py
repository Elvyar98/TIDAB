import math

def f(x):
    #return math.exp(-x**2)
    return x**2


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

def konvergenstabell(method, method_name, f, a, b, exakt):
    print(method_name)
    
    n = 1
    prev_error = None
    steps = 10

    for _ in range(steps):
        h = (b - a) / n
        approx = method(f, a, b, n)
        error = abs(approx - exakt)
        

        if prev_error is not None and error != 0:
            rate = math.log(prev_error / error) / math.log(2)
            print(f"h: {h:.3e}, int = {approx:.12f}, err = {error:.2e}, rate={rate:.2f}")
        else:
            print(f"h: {h:.3e}, int = {approx:.12f}, err = {error:.2e}")
        
        prev_error = error
        n *= 2
    
    print(f"N = {n}\n")


def main():
    #exakt = math.sqrt(math.pi)/2 * math.erf(1)
    exakt = 8/3
    # f(x) = e^(-x^2)
    konvergenstabell(rektangelmetoden, "Rektangel f(x)=e^(-x^2)", f, 0, 2, exakt)
    konvergenstabell(trapetsmetoden, "Trapets f(x)=e^(-x^2)", f, 0, 2, exakt)


if __name__ == "__main__":
    main()