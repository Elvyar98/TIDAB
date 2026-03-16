import numpy as np
import matplotlib.pyplot as plt

def f(x):     #returnerar funktionen. Tar emot en argument x som ska vara en float?
    return x - 3*np.cos(3*x) - 2
    #return x**2-2

def fprim(x):     #returnerar derivatan av funktionen. Tar emot en argument x
    return 1 + 9*np.sin(3*x)
    #return 2*x


# Implementerar newton_raphson metoden som en funktion. 
# Den accepterar f, fprim, ett starvärde på x, toleransen samt max antal iterationer
def newton_raphson(f, fprim, x0, tol, max_iter):
    x = float(x0)
    xs = [x]

    X = np.linspace(x0-3, x0+3, 1000)
    Y = f(X)

    plt.plot(X, Y, label="f(x)")
    plt.axhline(0, color='black', linewidth=0.5)  # x-axeln
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.title("Newton-Raphson visualisering")
    plt.grid(True)



    for iteration in range(1, max_iter): #Iterar till max interation, avbryter om fprim = 0
        if np.isclose(fprim(x), 0):
            print("Avbryter pga kritiskt punkt i x = ", x)
            break

        x_new = x - f(x)/fprim(x) #ett nytt x skapas och err blir absloutbeloppet av nya - gamla
        err = abs(x_new - x)
        #fel = abs(np.sqrt(2) - x_new)

        tangent_x = np.linspace(x, x_new, 100)
        tangent_y = f(x) + fprim(x)*(tangent_x - x)
        plt.plot(tangent_x, tangent_y, color='red', linestyle='--', label="Tangent" if iteration==1 else "")
        plt.plot([x_new, x_new], [0, f(x_new)], color='blue', linestyle=':', label="Vertikal" if iteration==1 else "")
        plt.text(x_new, 0, f"x{iteration}", fontsize=9, ha='center', va='bottom')

        if abs(f(x_new)) < tol or err < tol:
            x = x_new
            xs.append(x)
            print(f"Rot approx x ≈ {x_new} efter {iteration} iterationer. Error rate = {err}")
            break
        
        x = x_new
        xs.append(x)

    plt.legend()
    plt.show()

    #print( "Ingen konvergens efter", max_iter, "iterationer.")

def main():
    newton_raphson(f,fprim, 0.5, 1e-6,100)


if __name__ == "__main__":
    main()


#1
#Startvärdet = 2, iterationer = 5

#2 Minsta och största värden inom intervallet 0, 4
#Minsta rot. Startvärde = 0.6 , iterationer=4, err = 9.05e-12
#Största värden. Startvärden = 4, root=4.40, iterationer = 6, err=1.6e-6