import numpy as np
import matplotlib.pyplot as plt

def f(x):     #returnerar funktionen. Tar emot en argument x som ska vara en float?
    return x - 3*np.cos(3*x) - 2
    #return x**2-2


# Implementerar sekant metoden som en funktion. 
# Den accepterar f, fprim, ett starvärde på x, toleransen samt max antal iterationer
def sekant(f,x0, x1, tol, max_iter):
    x_0 = x0
    x_1 = x1
    xs = [x_0]

    X = np.linspace(x_0-3, x_0+3, 1000)
    Y = f(X)

    plt.plot(X, Y, label="f(x)")
    plt.axhline(0, color='black', linewidth=0.5)  # x-axeln
    plt.xlabel("x")
    plt.ylabel("f(x)")
    plt.title("Sekant visualisering")
    plt.grid(True)



    for iteration in range(1, max_iter): #Iterar till max interation, avbryter om fprim = 0
        if np.isclose(f(x_0), f(x_1)):
            print("Avbryter pga kritiskt punkt")
            break

        x_new = x_0 - f(x_0) * (x_0 - x_1) / (f(x_0) - f(x_1)) #ett nytt x skapas och err blir absloutbeloppet av nya - gamla
        err = abs(x_new - x_0)
        fel = abs(np.sqrt(2) - x_new)

        tangent_x = np.linspace(x_0, x_new, 100)
        tangent_y = f(x_0) + ((f(x_0) - f(x_1)) / (x_0 - x_1))*(tangent_x - x_0)
        plt.plot(tangent_x, tangent_y, color='red', linestyle='--', label="Tangent" if iteration==1 else "")
        plt.plot([x_new, x_new], [0, f(x_new)], color='blue', linestyle=':', label="Vertikal" if iteration==1 else "")
        plt.text(x_new, 0, f"x{iteration}", fontsize=9, ha='center', va='bottom')

        if abs(f(x_new)) < tol or err < tol:
            x_0 = x_new
            xs.append(x_0)
            print(f"Rot approx x ≈ {x_new} efter {iteration} iterationer. Error rate = {err}")
            break
        
        x_1 = x_0
        x_0 = x_new
        xs.append(x_0)

    plt.legend()
    plt.show()


def main():
    sekant(f, 4.4, 4.6 , 1e-6,100)


if __name__ == "__main__":
    main()



#1
#Startvärden = 1, 2, iterationer = 5, err = 2.12e-06

#2 Minsta och största värden inom intervallet -3, 3
#Minsta rot. Startvärden = 0.5 1 , iterationer=5, err = 6.24e-12
#Största värden. Startvärden = 4 5, root=3,89, iterationer = 7, err=1.10e-6