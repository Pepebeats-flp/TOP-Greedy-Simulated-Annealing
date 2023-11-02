#define GCC_COMPILER (defined(__GNUC__) && !defined(__clang__))
#if GCC_COMPILER
    #include <bits/stdc++.h>
#else
    #include "stdc++.h"
#endif


using namespace std;


// Debemos abrir las instanias de archivos .txt de la carpeta "/instancias/Set_21"
// y guardar los datos en una estructura de datos para luego poder trabajar con ellos.


// Estructura de datos para almacenar los datos de cada instancia
struct instancia {
    int n; // cantidad de puntos de control
    int m; // cantidad de rutas a generar
    float tmax; // tiempo límite
    vector<float> x; // coordenadas x de los puntos de control
    vector<float> y; // coordenadas y de los puntos de control
    vector<int> s; // puntaje de cada punto de control (s_i) i = 1, ..., n
};

// Función para leer los datos de una instancia
instancia leer_instancia(string nombre_ruta){
    ifstream archivo(nombre_ruta.c_str());
    instancia inst;
    cout << "Leyendo archivo: " << nombre_ruta << endl;
    if (archivo.is_open()){
        string linea;

        getline(archivo, linea);
        inst.n = stoi(linea.substr(2));

        getline(archivo, linea);
        inst.m = stoi(linea.substr(2));        

        getline(archivo, linea);
        inst.tmax = stof(linea.substr(5));

        archivo.close();

        cout << "Archivo leído correctamente" << endl;
    }
    else {
        cout << "No se pudo abrir el archivo" << endl;
    }
    return inst;
}

        
// Función para imprimir los datos de una instancia
void imprimir_instancia(instancia inst){
    cout << "n: " << inst.n << endl;
    cout << "m: " << inst.m << endl;
    cout << "tmax: " << inst.tmax << endl;
}


string nombre_ruta = "instancias/Set_21/p2.2.a.txt";

int main(){
    instancia inst = leer_instancia(nombre_ruta);
    imprimir_instancia(inst);
    return 0;
}