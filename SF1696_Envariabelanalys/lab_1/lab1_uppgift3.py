import numpy as np
import matplotlib.pyplot as plt

#Funktion och derivata
def f(x):
    return x**2 - 2
    #return x**2

def fprim(x):
    return 2*x

def compare_methods(f, fprim, x0_nr, x0_sec, x1_sec, tol=1e-6, max_iter=20):
   
    # Newton-Raphson
    x_nr = float(x0_nr)
    errs_nr = []
    print("\n--- Newton-Raphson ---")
    for iteration in range(1, max_iter+1):
        if np.isclose(fprim(x_nr), 0):
            print("NR avbryter pga kritisk punkt")
            break
        x_new_nr = x_nr - f(x_nr)/fprim(x_nr)
        err_nr = abs(x_new_nr - x_nr)
        errs_nr.append(err_nr)
        
        if iteration >= 3:
            rate = np.log(errs_nr[-1]/errs_nr[-2]) / np.log(errs_nr[-2]/errs_nr[-3])
            print(f"NR Iter {iteration:3d}: x = {x_new_nr:.12f}, err = {err_nr:.2e}, rate = {rate:.2f}")
        else:
            print(f"NR Iter {iteration:3d}: x = {x_new_nr:.12f}, err = {err_nr:.2e}")
        
      
        
        if err_nr < tol:
            x_nr = x_new_nr
            break
        x_nr = x_new_nr
    
    # Sekantmetoden
    x_prev_sec = float(x0_sec)
    x_sec = float(x1_sec)
    errs_sec = []

    print("\n--- Sekantmetoden ---")
    for iteration in range(1, max_iter+1):
        if np.isclose(f(x_sec) - f(x_prev_sec), 0):
            print("Sekant avbryter pga division med 0")
            break

        x_new_sec = x_sec - f(x_sec)*(x_sec - x_prev_sec)/(f(x_sec) - f(x_prev_sec))
        err_sec = abs(x_new_sec - x_sec)
        errs_sec.append(err_sec)
        
        if iteration >= 3:
            rate = np.log(errs_sec[-1]/errs_sec[-2]) / np.log(errs_sec[-2]/errs_sec[-3])
            print(f"Sek Iter {iteration:3d}: x = {x_new_sec:.12f}, err = {err_sec:.2e}, rate = {rate:.2f}")
        else:
            print(f"Sek Iter {iteration:3d}: x = {x_new_sec:.12f}, err = {err_sec:.2e}")
       
        
        if err_sec < tol:
            x_prev_sec = x_sec
            x_sec = x_new_sec
            break
        x_prev_sec, x_sec = x_sec, x_new_sec
    
   
    return x_nr, x_new_sec

# ---- Kör metoderna på samma graf ----
rot_nr, rot_sec = compare_methods(f, fprim, x0_nr=2, x0_sec=1, x1_sec=2, tol=1e-8, max_iter=20)
print(f"\nSlutlig rot NR: {rot_nr:.12f}")
print(f"Slutlig rot Sekant: {rot_sec:.12f}")