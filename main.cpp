//#include <bits/stdc++.h>
#include "stdc++.h"
using namespace std;

// Estructura de datos para almacenar los datos de cada instancia
struct instancia {
    int n; // cantidad de puntos de control
    int m; // cantidad de rutas(equipos) a generar
    float tmax; // tiempo límite
    vector<float> x; // coordenadas x de los puntos de control
    vector<float> y; // coordenadas y de los puntos de control
    vector<int> s; // puntaje de cada punto de control (s_i) i = 1, ..., n
};

// Estructura de datos para almacenar los datos de cada nodo
struct nodo{
    int id;
    float x;
    float y;
    int s;
};

// Estructura de datos para almacenar los datos de cada ruta
struct ruta{
    vector<nodo> nodos;
    float distancia;
    int puntaje;
};


// Función para leer los datos de una instancia y almacenarlos en una estructura
instancia leer_instancia(string nombre_ruta){
    instancia inst;
    ifstream archivo(nombre_ruta.c_str());
    if (archivo.is_open()){
        string n, m, tmax;

        getline(archivo, n);
        getline(archivo, m);
        getline(archivo, tmax);

        n = n.substr(2, n.length()-1);
        m = m.substr(2, m.length()-1);
        tmax = tmax.substr(5, tmax.length()-1);

        inst.n = stoi(n);
        inst.m = stoi(m);
        inst.tmax = stof(tmax);

        string linea;
        while (getline(archivo, linea)){
            stringstream ss(linea);
            string x, y, s;
            getline(ss, x, '\t');
            getline(ss, y, '\t');
            getline(ss, s, '\t');
            inst.x.push_back(stof(x));
            inst.y.push_back(stof(y));
            inst.s.push_back(stoi(s));
        }
        archivo.close();
    }
    else cout << "No se pudo abrir el archivo" << endl;
    return inst;
}
       
// Función para crear los nodos iniciales de la instancia
vector<nodo> crear_nodos(instancia inst){
    vector<nodo> nodos;
    for (int i = 0; i < inst.n; i++){
        nodo nodo_actual;
        nodo_actual.id = i+1;
        nodo_actual.x = inst.x[i];
        nodo_actual.y = inst.y[i];
        nodo_actual.s = inst.s[i];
        nodos.push_back(nodo_actual);
    }
    return nodos;
}

// Función para calcular la distancia/tiempos entre dos puntos
float distancia(float x1, float y1, float x2, float y2){
    return sqrt(pow(x1-x2, 2) + pow(y1-y2, 2));
}

// Función para calcular la distancia/tiempos total de una ruta
float distancia_total(instancia inst, vector<int> ruta){
    float distancia_total = 0;
    for (int i = 0; i < ruta.size()-1; i++){
        distancia_total += distancia(inst.x[ruta[i]], inst.y[ruta[i]], inst.x[ruta[i+1]], inst.y[ruta[i+1]]);
    }
    return distancia_total;
}

// Función para calcular el puntaje total de una ruta
int puntaje_total(instancia inst, vector<int> ruta){
    int puntaje_total = 0;
    for (int i = 0; i < ruta.size(); i++){
        puntaje_total += inst.s[ruta[i]];
    }
    return puntaje_total;
}



int main(){
    string nombre_ruta = "instancias/Set_21/p2.2.a.txt";

    instancia inst = leer_instancia(nombre_ruta);
    vector<nodo> nodos_iniciales = crear_nodos(inst);

    for (int i = 0; i < nodos_iniciales.size(); i++){
        printf("id: %d, x: %f, y: %f, s: %d\n", nodos_iniciales[i].id, nodos_iniciales[i].x, nodos_iniciales[i].y, nodos_iniciales[i].s);
    }

    return 0;
}