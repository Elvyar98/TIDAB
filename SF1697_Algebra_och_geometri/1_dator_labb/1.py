import numpy as np

def gauss_elimination(A, b):
    #Lös systemet Ax = b med Gauss-elimination efter lämplig sortering.
    #Returnerar lösningen x.
    #Kopior av A och b så originalen bevaras

    A = A.astype(float).copy()
    b = b.astype(float).copy()
    n = len(b)

    # Eliminering nedanför varje pivåvariabel
    for i in range(n):
        # För att unvdika division med noll eller små värden i onödan gör vi följande:
        # Hitta raden med största absolutvärde i kolumn i bland raderna i,…,n-1 och lägger den på rad i
        # Först hittas i vilken rad det största värdet finns
        maxrow=i
        for j in range(i+1,n):
            if abs(A[j,i])>abs(A[maxrow,i]):
                maxrow=j
        #Om inte raden är på plats i så byter vi plats både i A och i b   
        if maxrow!=i:
                A[[i,maxrow]]=A[[maxrow,i]]
                b[[i, maxrow]] = b[[maxrow, i]]

        # Här ska du lägga in elimination av rad i nedanför varje pivåvariabel, detta ska till slut
        # resultera i ett trappformat system/trappformad matris. Kom ihåg att uppdatera både A och b.
        for j in range(i+1,n):
            faktor = A[j,i] / A[i,i]
            A[j] = A[j] - faktor * A[i]
            b[j] = b[j] - faktor * b[i]
      
    # Här ska du sedan lägga in återsubstitution, mao gå nedifrån och lösa ut de obekanta steg för steg
        
    x = np.zeros(n)
    for i in range(n-1, -1, -1):
        summa = 0
        for j in range(i+1, n):
            summa += A[i,j]*x[j]
        x[i] = (b[i] - summa) / A[i,i]
        
        
    return x

def check_solution(A, x, b, tol=1e-6):
     #Kontrollera här om Ax ≈ b i norm med toleransen tol (utan inbyggda matrisfunktioner)
     #Returnera True om lösningen är korrekt inom tolerans tol annars false

    A = A.astype(float).copy()
    b = b.astype(float).copy()
    n = len(b)

    acceptable = False

    Ax = np.zeros(n)
    for i in range(n):
        for j in range(n):
            Ax[i] += A[i,j]*x[j]

    
    norm = 0
    for i in range(len(Ax)):
        norm += (Ax[i] - b[i])**2
    
    felet = np.sqrt(norm)
    
    if  felet < tol:
         acceptable = True

    print("felet, normen av Ax-b: ",felet)
    return acceptable

A = np.array([[2, 1], [1, 3]], dtype=float)
b = np.array([5, 6], dtype=float)
x = gauss_elimination(A, b)
print("Lösning:", x)

print("Kontroll:", check_solution(A, x, b))