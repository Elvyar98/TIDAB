import numpy as np
import matplotlib.pyplot as plt

# ==================== MODIFIERAD UPPGIFT 1 ====================
def euler_forward_icke_homogen(a, f, x0, y0, x_end, h):
    """
    Euler framåt för icke-homogen ODE: dy/dx = a*y + f(x)
    """
    N = int((x_end - x0) / h)
    
    x_vals = np.zeros(N + 1)
    y_vals = np.zeros(N + 1)
    
    x_vals[0] = x0
    y_vals[0] = y0
    x = x0
    y = y0
    
    for i in range(N):
        y = y + h * (a * y + f(x))
        x = x + h
        x_vals[i+1] = x
        y_vals[i+1] = y
    
    return x_vals, y_vals

# ==================== EXAKT LÖSNNING ====================
def exact_solution_icke_homogen(a, x0, y0, x):
    """
    Exakt lösning för dy/dx = a*y + sin(x), y(0)=0
    """
    if a == 0:
        return -np.cos(x) + np.cos(x0) + y0
    else:
        return (np.sin(x) - a*np.cos(x) - (np.sin(x0) - a*np.cos(x0))*np.exp(a*(x-x0))) / (1 + a**2) + y0*np.exp(a*(x-x0))

# ==================== STABILITETSANALYS ====================
def check_stability(a, h):
    """
    Kontrollerar stabilitetsvillkoret
    """
    stability_value = abs(1 + a * h)
    is_stable = stability_value <= 1
    return stability_value, is_stable

def get_stability_limit(a):
    """
    Beräknar teoretisk stabilitetsgräns
    """
    if a < 0:
        return 2 / abs(a)
    elif a == 0:
        return float('inf')
    else:
        return 0

# ==================== PLOTTING ====================
def plot_solutions(a, x0, y0, x_end, h_values):
    """
    Plottar lösningar för olika steglängder
    """
    f = lambda x: np.sin(x)
    
    # Skapa figur med 2x2 subplots
    fig, axes = plt.subplots(2, 2, figsize=(12, 10))
    axes = axes.flatten()
    
    # Exakt lösning
    x_exact = np.linspace(x0, x_end, 1000)
    y_exact = exact_solution_icke_homogen(a, x0, y0, x_exact)
    
    # Plotta för varje h
    for idx, (ax, h) in enumerate(zip(axes[:3], h_values[:3])):
        # Numerisk lösning
        x_num, y_num = euler_forward_icke_homogen(a, f, x0, y0, x_end, h)
        
        # Kontrollera stabilitet
        stability_value, is_stable = check_stability(a, h)
        
        # Plotta
        ax.plot(x_exact, y_exact, 'b-', label='Exakt lösning', linewidth=2, alpha=0.7)
        ax.plot(x_num, y_num, 'ro-', label=f'Euler, h={h}', markersize=4)
        ax.set_title(f'h = {h} ({"Stabil" if is_stable else "Instabil"})')
        ax.set_xlabel('x')
        ax.set_ylabel('y')
        ax.legend()
        ax.grid(True, alpha=0.3)
    
    # Stabilitetsanalys i sista subplotten
    ax = axes[3]
    h_test = np.linspace(0.1, 5, 50) if a < 0 else np.linspace(0.1, 2, 50)
    errors = []
    
    for h in h_test:
        x_num, y_num = euler_forward_icke_homogen(a, f, x0, y0, x_end, h)
        y_exact_at_points = exact_solution_icke_homogen(a, x0, y0, x_num)
        error = np.max(np.abs(y_num - y_exact_at_points))
        errors.append(error)
    
    ax.plot(h_test, errors, 'bo-', label='Maxfel')
    ax.set_xlabel('Steglängd h')
    ax.set_ylabel('Maximalt fel')
    ax.set_title(f'Stabilitetsanalys för a={a}')
    ax.legend()
    ax.grid(True, alpha=0.3)
    
    plt.suptitle(f'Lösning av dy/dx = {a}y + sin(x), y({x0})={y0}', fontsize=14)
    plt.tight_layout()
    plt.show()

# ==================== RESULTATVISNING ====================
def print_results(a, x0, y0, x_end, h_values):
    """
    Skriver ut numeriska resultat
    """
    f = lambda x: np.sin(x)
    
    print("\n" + "="*60)
    print("NUMERISKA RESULTAT:")
    print("="*60)
    print(f"{'h':<10} {'y({x_end})':<20} {'Stabilitet':<15}")
    print("-"*60)
    
    for h in h_values:
        x_num, y_num = euler_forward_icke_homogen(a, f, x0, y0, x_end, h)
        y_numerisk = y_num[-1]
        
        stability_value, is_stable = check_stability(a, h)
        stability_str = f"{'Stabil' if is_stable else 'Instabil'}"
        
        print(f"{h:<10.2f} {y_numerisk:<20.8f} {stability_str:<15}")

# ==================== GRÄNSSÖKNING ====================
def find_stability_boundary(a, x0, y0, x_end):
    """
    Hittar empirisk stabilitetsgräns
    """
    f = lambda x: np.sin(x)
    
    # Sök gränsen
    h_low, h_high = 0.1, 5.0
    tolerance = 0.01
    
    for _ in range(20):  # Max 20 iterationer
        h_mid = (h_low + h_high) / 2
        stability_value, is_stable = check_stability(a, h_mid)
        
        if is_stable:
            h_low = h_mid
        else:
            h_high = h_mid
        
        if (h_high - h_low) < tolerance:
            break
    
    return (h_low + h_high) / 2

# ==================== MAIN FUNKTION ====================
def main():
    """
    Huvudfunktion för Uppgift 3 - Minimistisk
    """
    print("="*60)
    print("UPPGIFT 3: dy/dx = a*y + sin(x)")
    print("="*60)
    
    # 1. Fråga om parametrar
    print("\n--- MATTA IN PARAMETRAR ---")
    
    a_input = input("Ange parameter a (standard -0.5): ").strip()
    a = float(a_input) if a_input else -0.5
    
    x0_input = input("Ange startpunkt x0 (standard 0): ").strip()
    x0 = float(x0_input) if x0_input else 0
    
    y0_input = input("Ange startvärde y0 (standard 0): ").strip()
    y0 = float(y0_input) if y0_input else 0
    
    x_end_input = input("Ange slutpunkt x_N (standard 10): ").strip()
    x_end = float(x_end_input) if x_end_input else 10
    
    # 2. Välj steglängder
    print("\n--- VÄLJ STEGLÄNGDER ---")
    print("1. Använd standardvärden (0.2, 0.5, 2.0)")
    print("2. Ange egna värden")
    
    choice = input("Ditt val (1 eller 2): ").strip()
    
    if choice == "1":
        h_values = [0.2, 0.5, 2.0]
    else:
        h_input = input("Ange steglängder (separera med mellanslag): ").strip()
        h_values = [float(x) for x in h_input.split()]
    
    # 3. Visa problemformulering
    print("\n" + "="*60)
    print(f"PROBLEM: dy/dx = {a}y + sin(x), y({x0}) = {y0}")
    print(f"Intervall: [{x0}, {x_end}]")
    print(f"Steglängder: {h_values}")
    print("="*60)
    
    # 4. Beräkna och visa stabilitetsgränser
    print("\n--- STABILITETSANALYS ---")
    theoretical_limit = get_stability_limit(a)
    
    if a < 0:
        print(f"Teoretisk stabilitetsgräns: h ≤ {theoretical_limit:.4f}")
        empirical_limit = find_stability_boundary(a, x0, y0, x_end)
        print(f"Empirisk stabilitetsgräns: h ≈ {empirical_limit:.4f}")
    elif a == 0:
        print("a = 0: Alltid stabilt för alla h")
    else:
        print(f"a > 0: Alltid instabilt för h > 0")
    
    # 5. Visa stabilitet för varje steglängd
    print("\nStabilitetskontroll för valda steglängder:")
    for h in h_values:
        stability_value, is_stable = check_stability(a, h)
        status = "✓ STABIL" if is_stable else "✗ INSTABIL"
        print(f"  h = {h}: |1 + {a}*{h}| = {stability_value:.3f} -> {status}")
    
    # 6. Skriv ut numeriska resultat
    print_results(a, x0, y0, x_end, h_values)
    
    # 7. Plotta lösningar
    print("\n--- PLOTTAR LÖSNINGAR ---")
    plot_solutions(a, x0, y0, x_end, h_values)
    
    print("\n" + "="*60)
    print("ANALYS KLAR!")
    print("="*60)

# Kör programmet
if __name__ == "__main__":
    main()