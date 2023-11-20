# TOP-Greedy-Simulated-Annealing 
- José Pinto Muñoz - 202073559-K

Para poder compilar y ejecutar el programa se debe tener correr el siguiente comando en la terminal:

    make run

En caso de querer compilar y ejecutar el programa de forma manual se debe correr el siguiente comando en la terminal:
Para compilar:

    g++ -o main.o main.cpp 

Para ejecutar:

    ./main.o

Se debe tener los archivos con las instancias en una carpeta llamada `instancias`. Para seleccionar  que archivo abrir se debe cambiar el nombre del archivo en la linea 196 del archivo `main.cpp`:
    
        string nombre_ruta = "instancias/Set_21/p2.2.b.txt";

    
Se debe tener en cuanta que se importa el archivo "stdc++.h" para poder utilizar las librearias de C++. En el caso de no poder importar el archivo se debe descomeentar la linea 1 del archivo `main.cpp` y comentar la linea 2 del mismo archivo.
    
    #include <bits/stdc++.h>
    //#include "stdc++.h"
    

El resultado de la ejecución del programa se entrega en un archivo llamado `solution.txt`. En el archivo se puede ver el resultado de la ejecución del programa con el formato que se pide en el enunciado. Pero además se imprime en la consola la lectura de la instancia, la solución inicial, las posibles soluciones y la solución final seleccionada con sus puntajes y evaluaciones correspondientes.