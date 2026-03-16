import numpy as np

#Implemterar Euler framåt-metoden för ODE: dy/dx = a*y + f(x)
#   Parametrar:
#   f: funktion f(x) (kan vara None om a är given och vi använder homogen form)
#   x0: startpunkt
#   y0: startvärde y(x0)
#   xN: slutpunkt
#   N: antal steg (om None, beräknas från h)
#   h: steglängd (om None, beräknas från N)
#   a: parameter a i ekvationen (kan vara None)

def euler_forward_homogen(a, x0, y0, x_end, h):
    """
    Euler framåt för homogen ODE: dy/dx = a*y
    Returnerar både sista värdet och hela lösningen
    """
    N = int((x_end - x0) / h)
    
    # Skapa arrays
    x_vals = np.zeros(N + 1)
    y_vals = np.zeros(N + 1)
    
    # Initialvärden
    x_vals[0] = x0
    y_vals[0] = y0
    x = x0
    y = y0
    
    # Euler framåt iteration
    for i in range(N):
        y = y + h * (a * y)  # Homogen: f(x)=0
        x = x + h
        
        # Spara värdena
        x_vals[i+1] = x
        y_vals[i+1] = y
    
    return y, x_vals, y_vals

def exact_solution_homogen(a, x0, y0, x):
    """
    Exakt lösning för dy/dx = a*y, y(x0)=y0
    """
    return y0 * np.exp(a * (x - x0))

# ==================== KONVERGENSORDNINGS-FUNKTIONER ====================
def berakna_fel_och_konvergens(a, x0, y0, x_end, h_list):
    """
    Beräknar fel och konvergensordning för olika steglängder
    """
    results = []
    prev_error = None
    
    for h in h_list:
        # Beräkna numerisk lösning
        y_num, _, _ = euler_forward_homogen(a, x0, y0, x_end, h)
        
        # Beräkna exakt lösning
        exact = exact_solution_homogen(a, x0, y0, x_end)
        
        # Beräkna fel
        error = abs(y_num - exact)
        
        # Beräkna konvergensordning om möjligt
        if prev_error is not None and error > 0:
            konv_ordning = np.log2(prev_error / error)
        else:
            konv_ordning = np.nan
        
        # Spara resultat
        results.append({
            'h': h,
            'y_num': y_num,
            'exact': exact,
            'error': error,
            'konv_ordning': konv_ordning
        })
        
        prev_error = error
    
    return results

def konvergensanalys(a, x0, y0, x_end, start_h=0.5, antal_halveringar=6):
    """
    Utför konvergensanalys genom successiv halvering av steglängd
    """
    print(f"{'h':<12} {'y_num':<20} {'Fel':<15} {'Konv. ordning':<15}")
    print("-"*60)
    
    results = []
    h = start_h
    
    for i in range(antal_halveringar):
        # Beräkna numerisk lösning
        y_num, _, _ = euler_forward_homogen(a, x0, y0, x_end, h)
        
        # Beräkna exakt lösning
        exact = exact_solution_homogen(a, x0, y0, x_end)
        
        # Beräkna fel
        error = abs(y_num - exact)
        
        # Beräkna konvergensordning (förutom första gången)
        if i > 0 and error > 0:
            prev_error = results[i-1]['error']
            konv_ordning = np.log2(prev_error / error)
        else:
            konv_ordning = np.nan
        
        # Spara resultat
        results.append({
            'h': h,
            'y_num': y_num,
            'exact': exact,
            'error': error,
            'konv_ordning': konv_ordning
        })
        
        # Skriv ut
        if np.isnan(konv_ordning):
            print(f"{h:<12.6f} {y_num:<20.12f} {error:<15.2e} {'N/A':<15}")
        else:
            print(f"{h:<12.6f} {y_num:<20.12f} {error:<15.2e} {konv_ordning:<15.4f}")
        
        # Halvera för nästa iteration
        h = h / 2
    
    return results

# ==================== STABILITETSANALYS-FUNKTIONER ====================
def stabilitetsvillkor(a, h):
    """
    Kontrollerar stabilitetsvillkoret för Euler framåt
    Returnerar (stabilitetsvärde, är_stabil)
    """
    stabilitetsvarde = abs(1 + a * h)
    ar_stabil = stabilitetsvarde <= 1
    return stabilitetsvarde, ar_stabil

def berakna_stabilitetsgrans(a):
    """
    Beräknar teoretisk stabilitetsgräns för Euler framåt
    """
    if a < 0:
        return 2 / abs(a)
    elif a == 0:
        return float('inf')  # Alltid stabilt
    else:  # a > 0
        return 0  # Aldrig stabilt för h > 0

def stabilitetsanalys(a, h_list):
    """
    Utför stabilitetsanalys för olika steglängder
    """
    print(f"\nStabilitetsvillkor: |1 + a*h| ≤ 1")
    print("-"*60)
    
    for h in h_list:
        stabilitetsvarde, ar_stabil = stabilitetsvillkor(a, h)
        status = "STABIL ✓" if ar_stabil else "INSTABIL ✗"
        print(f"h = {h:<6} | |1 + {a}*{h}| = {stabilitetsvarde:<8.4f} -> {status}")
    
    # Visa teoretisk gräns
    grans = berakna_stabilitetsgrans(a)
    
    if a < 0:
        print(f"\nTeoretisk stabilitetsgräns (för a < 0):")
        print(f"h ≤ {grans:.4f}")
    elif a == 0:
        print(f"\nFör a = 0:")
        print(f"Metoden är alltid stabil (h ≤ ∞)")
    else:  # a > 0
        print(f"\nFör a > 0:")
        print(f"Metoden är alltid instabil för h > 0")
    
    return grans

# ==================== VISNINGS-FUNKTIONER ====================
def visa_berakningar(results):
    """
    Visar beräkningar i en snygg tabell
    """
    print(f"{'h':<10} {'N':<6} {'y_num':<20} {'Fel':<15} {'Rel. fel':<15}")
    print("-"*70)
    
    for res in results:
        h = res['h']
        N = int(1/h * 5) if h > 0 else 0  # Ungefärligt N för x_end=5
        y_num = res['y_num']
        error = res['error']
        exact = res['exact']
        rel_error = error / abs(exact) if exact != 0 else error
        
        print(f"{h:<10.6f} {N:<6} {y_num:<20.12f} {error:<15.2e} {rel_error:<15.2e}")

def visa_hel_losning(a, x0, y0, x_end, h):
    """
    Visar hela lösningen (alla x- och y-värden)
    """
    y_num, x_vals, y_vals = euler_forward_homogen(a, x0, y0, x_end, h)
    
    print(f"\nHELA LÖSNINGEN för h = {h}:")
    print("-"*40)
    print(f"{'x':<10} {'y_num':<15} {'y_exakt':<15}")
    print("-"*40)
    
    # Visa var 5:e värde om det är för många punkter
    step = max(1, len(x_vals) // 20)  # Visa max 20 rader
    
    for i in range(0, len(x_vals), step):
        x_val = x_vals[i]
        y_val = y_vals[i]
        y_exakt = exact_solution_homogen(a, x0, y0, x_val)
        
        print(f"{x_val:<10.4f} {y_val:<15.8f} {y_exakt:<15.8f}")
    
    # Visa sista värdet alltid
    if len(x_vals) - 1 not in range(0, len(x_vals), step):
        x_val = x_vals[-1]
        y_val = y_vals[-1]
        y_exakt = exact_solution_homogen(a, x0, y0, x_val)
        print("..." + " "*7 + "...")
        print(f"{x_val:<10.4f} {y_val:<15.8f} {y_exakt:<15.8f}")
    
    print("-"*40)

# ==================== MAIN FUNKTION ====================
def main():
    """
    Huvudfunktion som koordinerar alla delar
    """
    print("="*60)
    print("UPPGIFT 2: Euler framåt för dy/dx = a*y")
    print("="*60)
    
    # 1. Fråga användaren om parametrar
    print("\n--- MATTA IN PARAMETRAR ---")
    
    # Parameter a
    a_input = input("Ange parameter a (standard -2): ").strip()
    a = float(a_input) if a_input else -2
    
    # Startvillkor
    x0_input = input("Ange startpunkt x0 (standard 0): ").strip()
    x0 = float(x0_input) if x0_input else 0
    
    y0_input = input("Ange startvärde y0 (standard 1): ").strip()
    y0 = float(y0_input) if y0_input else 1
    
    # Slutpunkt
    x_end_input = input("Ange slutpunkt x_N (standard 5): ").strip()
    x_end = float(x_end_input) if x_end_input else 5
    
    print("\n" + "="*60)
    print(f"PROBLEM: dy/dx = {a}*y, y({x0}) = {y0}")
    print(f"Beräknar y({x_end}) med Euler framåt")
    print("="*60)
    
    # 2. Visa exakt lösning
    exact = exact_solution_homogen(a, x0, y0, x_end)
    print(f"\nEXAKT LÖSNING:")
    print(f"y(x) = {y0} * exp({a}*(x-{x0}))")
    print(f"y({x_end}) = {exact:.12f}")
    
    # 3. Steglängder från uppgiften
    h_list = [0.5, 0.25, 0.125, 0.0625]
    
    # 4. Beräkningar med olika steglängder
    print("\n" + "="*60)
    print("BERÄKNINGAR MED OLIKA STEGLÄNGDER:")
    print("="*60)
    
    results = berakna_fel_och_konvergens(a, x0, y0, x_end, h_list)
    visa_berakningar(results)
    
    # 5. Konvergensanalys
    print("\n" + "="*60)
    print("KONVERGENSANALYS (SUCCESSIV HALVERING):")
    print("="*60)
    
    konv_results = konvergensanalys(a, x0, y0, x_end, start_h=0.5, antal_halveringar=6)
    
    # 6. Stabilitetsanalys
    print("\n" + "="*60)
    print("STABILITETSANALYS:")
    print("="*60)
    
    stabilitetsgrans = stabilitetsanalys(a, h_list)
    
    print("\n" + "="*60)
    print("ANALYS KLAR!")
    print("="*60)
    
    # 7. Fråga om användaren vill se hela lösningen för ett specifikt h
    while True:
        visa_hel = input("\nVill du se hela lösningen för en specifik steglängd? (j/n): ").strip().lower()
        
        if visa_hel in ['j', 'ja', 'y', 'yes']:
            try:
                h_val = float(input(f"Ange steglängd (t.ex. 0.5, 0.25, etc): "))
                visa_hel_losning(a, x0, y0, x_end, h_val)
            except ValueError:
                print("Ogiltig inmatning!")
                break
        else:
            break

# Kör programmet
if __name__ == "__main__":
    main()