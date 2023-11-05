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

// Estructura de datos para almacenar las rutas
struct ruta{
    std::vector<nodo> waypoints;
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

        // Guardamos los datos en la estructura instancia
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
            // Guardamos los datos en la estructura instancia
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

// Función para calcular la tiempos entre dos puntos (pitágoras)
float calculateTravelTime(nodo nodo1, nodo nodo2){
    float x1 = nodo1.x;
    float y1 = nodo1.y;
    float x2 = nodo2.x;
    float y2 = nodo2.y;
    float distancia = sqrt(pow(x2-x1, 2) + pow(y2-y1, 2));
    return distancia;
}


// Función para calcular la tiempos total de una ruta
float calculateTotalTravelTime(std::vector<nodo> waypoints){
    float total_time = 0;
    for (int i = 0; i < waypoints.size()-1; i++){
        total_time += calculateTravelTime(waypoints[i], waypoints[i+1]);
    }
    return total_time;
}

// Función para calcular el puntaje total de una ruta
int calculateTotalScore(std::vector<nodo> waypoints){
    int total_score = 0;
    for (int i = 0; i < waypoints.size(); i++){
        total_score += waypoints[i].s;
    }
    return total_score;
}


// Función de Evaluación (calcular el puntaje total de todas las rutas 
// y penalizar si se excede el tiempo límite)
float evaluate(std::vector<ruta> rutas, instancia inst){
    float total_score = 0;
    float total_time = 0;
    for (int i = 0; i < rutas.size(); i++){
        total_score += calculateTotalScore(rutas[i].waypoints);
        total_time += calculateTotalTravelTime(rutas[i].waypoints);
    }
    if (total_time > inst.tmax){
        total_score -= 10000;
    }
    return total_score;
}


// Función para crear una solución inicial con Greedy
std::vector<ruta> greedy(instancia inst, std::vector<nodo> nodos_iniciales){
    std::vector<ruta> rutas;

    // Creamos las rutas iniciales con el nodo 1 en cada una
    for (int i = 0; i < inst.m; i++){
        ruta ruta_actual;
        ruta_actual.waypoints.push_back(nodos_iniciales[0]);
        ruta_actual.puntaje = 0;
        rutas.push_back(ruta_actual);
    }

    // Creamos un vector de nodos disponibles
    std::vector<nodo> nodos_disponibles;
    for (int i = 1; i < nodos_iniciales.size(); i++){
        nodos_disponibles.push_back(nodos_iniciales[i]);
    }


    // Agregamos los nodos a las rutas hasta el nodo n-1
    for (int i = 0; i < nodos_iniciales.size()-1; i++){
        // Recorremos cada ruta
        for (int j = 0;j<inst.m;j++){
            int puntaje_max = 0;
            int indice_max = 0;
            // Recorremos cada nodo disponible
            for (int k = 0; k < nodos_disponibles.size(); k++){

                // Definimos la posible solución
                std::vector<nodo> posible_solucion = rutas[j].waypoints;
                posible_solucion.push_back(nodos_disponibles[k]);

                // Agregamos el nodo final al final de la ruta
                posible_solucion.push_back(nodos_iniciales[nodos_iniciales.size()-1]);

                // Evaluamos la posible solución
                float puntaje = evaluate({{posible_solucion, 0}}, inst);

                // Si el puntaje es mayor al máximo, actualizamos el puntaje máximo y el índice máximo
                if (puntaje > puntaje_max){ 
                    puntaje_max = puntaje;
                    indice_max = k;
                }
            }
            // Si el puntaje máximo es mayor a 0, agregamos el nodo al final de la ruta
            // y lo eliminamos de los nodos disponibles
            if (puntaje_max > 0){
                rutas[j].waypoints.push_back(nodos_disponibles[indice_max]);
                rutas[j].puntaje += nodos_disponibles[indice_max].s;
                nodos_disponibles.erase(nodos_disponibles.begin()+indice_max);
            }
        }
    }

    // Agregamos el nodo n a cada ruta si es que no está
    for (int i = 0; i < rutas.size(); i++){
        if (rutas[i].waypoints[rutas[i].waypoints.size()-1].id != nodos_iniciales[nodos_iniciales.size()-1].id){
            rutas[i].waypoints.push_back(nodos_iniciales[nodos_iniciales.size()-1]);
        }
    }

    return rutas;
}


int main(){

    string nombre_ruta = "instancias/Set_21/p2.2.b.txt";

    instancia inst = leer_instancia(nombre_ruta);
    vector<nodo> nodos_iniciales = crear_nodos(inst);

    printf("---------------------------------------------------------------\n");
    printf("Instancia: %s\n", nombre_ruta.c_str());
    printf("---------------------------------------------------------------\n");
    printf("n: %d\n", inst.n);
    printf("m: %d\n", inst.m);
    printf("tmax: %f\n", inst.tmax);
    printf("---------------------------------------------------------------\n");
    printf("Nodos iniciales:\n");
    printf("---------------------------------------------------------------\n");
    for (int i = 0; i < nodos_iniciales.size(); i++){
        printf("id: %d, x: %f, y: %f, s: %d\n", nodos_iniciales[i].id, nodos_iniciales[i].x, nodos_iniciales[i].y, nodos_iniciales[i].s);
    }
    printf("---------------------------------------------------------------\n");
    printf("Solución inicial:\n");
    printf("---------------------------------------------------------------\n");
    std::vector<ruta> rutas = greedy(inst, nodos_iniciales);
    float puntaje_total = 0;
    for (int i = 0; i < rutas.size(); i++){
        printf("RUTA %d\n", i+1);
        printf("Waypoints:\n");
        printf("    id: %d, x: %f, y: %f, s: %d, t: -- \n", rutas[i].waypoints[0].id, rutas[i].waypoints[0].x, rutas[i].waypoints[0].y, rutas[i].waypoints[0].s);
        for (int j = 1; j < rutas[i].waypoints.size(); j++){
            printf("    id: %d, x: %f, y: %f, s: %d, t: %f\n", rutas[i].waypoints[j].id, rutas[i].waypoints[j].x, rutas[i].waypoints[j].y, rutas[i].waypoints[j].s, calculateTravelTime(rutas[i].waypoints[j], rutas[i].waypoints[j-1]));
        }
        printf("Puntaje: %d\n", rutas[i].puntaje);
        printf("Tiempo: %f\n", calculateTotalTravelTime(rutas[i].waypoints));
        puntaje_total += rutas[i].puntaje;
    printf("---------------------------------------------------------------\n");
    }
    printf("Puntaje total: %d\n", (int)round(puntaje_total));
    printf("---------------------------------------------------------------\n");

    return 0;
}




