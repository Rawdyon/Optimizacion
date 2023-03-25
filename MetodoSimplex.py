#from tkinter import PhotoImage, Tk, Label ,Button, Entry 
from tkinter import PhotoImage, Tk, Label ,Button, Entry,PhotoImage, Toplevel
import os 
#import subprocess
rows = []

#Lista = [ "<=", "=",">="]


root = Tk()
altura = 600
ancho = 720
root.config(width=ancho, height=altura)

resP = Toplevel()
resP.state( newstate= "withdraw")

resP.config(width= 500, height=400)
Stonks = PhotoImage(file = "Stonks.png") 
NoStonks = PhotoImage(file = "NoStonks.png")

bg = PhotoImage(file = "Fondo.png") 
labelF = Label( root, image = bg) 
labelF.place(x=0,y=0)

# Crear caja de texto.
entry = Entry(root,width=5)
entry2 = Entry(root,width=5)
entry3 = Entry(root,width=5)
label = Label(root,text="variables:",foreground="#ffffff",background="#000000")
label2 = Label(root,text="restricciones:",foreground="#ffffff",background="#630101")
label3 = Label(root,text="Max o Min:",foreground="#ffffff",background="#630101")
label.place(x=30,y=23,width=50,height=10)
label2.place(x=150, y = 23, width= 80, height=10)
label3.place(x=270, y = 23, width= 80, height=10)

entry.place(x=80, y=20)
entry2.place(x = 230, y = 20)
entry3.place(x = 350, y = 20)

entry.insert(0,'0')
entry2.insert(0,'0')
entry3.insert(0,"Max")

def desplegar(n,m):
    posx = int( ancho/(n+1) )
    posy = int( altura/((m+1) ) )
    tam =  int(  ancho/((n+1)*10) ) 
    cols = []
    for j in range(n-2):
        e = Entry(root,width = tam) #Crea las entradas de texto de la función objetivo
        e.place( x=50 + j*posx, y= 50 + 0*posy) 
        cols.append(e) #Las agrega a el arreglo
    rows.append(cols) 

    for i in range(1,m):
        cols = []
        for j in range(n):
            e = Entry(root,width = tam) #Crea las entradas de texto de las restricciones
            if j == n-2:
                e.insert(0,"<=") # Estableces las restricicones como <= (Puede cambiar)
            e.place( x=50 + j*posx, y= 50 + i*posy) 
            cols.append(e) #Las agrega a el arreglo
        rows.append(cols) 

def imprime(n,m):
    with open("Entrada.txt","w") as f:
        if entry3.get() == "Max":
            f.write('0'+"\n")
        else:
            f.write('1'+"\n")
        f.write(str(m-1) + ' '+str(n-2) + "\n") #Escribe el numero de renglones y columnas
        for i in range(n-2):
            f.write(rows[0][i].get() + ' ')
        f.write("\n")
        for i in range(1,m):
            for j in range (n):
                if j == n-2:
                    #if i == 0:
                        #continue
                    if rows[i][j].get() == "<=":
                        f.write("3 ")
                    elif rows[i][j].get() == "=":
                        f.write("1 ")
                    elif rows[i][j].get() == ">=":
                        f.write("2 ")
                    else:
                        f.write("3 ")
                        return 
                else:
                    f.write(rows[i][j].get() + ' ') #Lee el texto en cada entrada y lo escribe en el .txt
            f.write("\n")
        #os.startfile("C:\\Users\\eder_\\OneDrive\\Escritorio\\Programación Lineal\\MetodoSimplex\\MetodoSimplex.exe")
        #os.getcwd()+"MetodoSimplex.exe"
        os.startfile(os.getcwd()+"\\\\MetodoSimplex.exe")
    with open("Resultado.txt","r") as f:
        res = f.read()
        coef = res.split()
        if int(coef[0]) == 1: #Significa que fue hay solución
            sol = float(coef[1])
            xres = [0.0]*(n-2)

            print("Z=",sol)
            #print("len=",len(coef))
            for j in range(2, len(coef) ,2):
                #print("j=",j,"coef j = ",coef[j]," coef j+1 =",coef[j+1])
                for i in range(n-2):
                    #print("i=",i," coef=",coef[j])
                    if i == int(coef[j]):
                        xres[i] = float(coef[j+1])
            for j in range(n-2):
                print("\nx"+str(j+1),xres[j])
            #rowsA = []
            labelS = Label(resP, image = Stonks)
            labelS.place(x=10,y=10)
            resP.state( newstate= "normal")

            e = Label(resP,text = "La solución es z = "+str(sol))
            e.place( x=50, y= 150) 
            for j in range(n-2):
                e = Label(resP,text = 'x'+str(j+1)+" = "+str(xres[j])) 
                e.place( x=50, y= 200 + j*20) 
            
        else:
            labelNS = Label(resP, image = NoStonks)
            if int(coef[0]) == 2:
                labelR = Label(resP,text="La solución es no acotada")
            elif int(coef[0]) == 3:
                labelR = Label(resP,text="Una variable de holgura no sale de la base")
            labelNS.place(x=0,y=0)
            labelR.place(x=100,y=50,width=100,height=10)

button = Button(root, text="siguiente",
        command= lambda : desplegar(int(entry.get()) +2 ,int(entry2.get()) +1) ) 
button.place(x=400, y=15)

button2 = Button(root, text="Optimizar", 
        command= lambda : imprime( int(entry.get()) +2, int(entry2.get()) +1) ) 
button2.place(x=600, y=500)

resP.mainloop()
root.mainloop()

