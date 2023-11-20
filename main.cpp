//#include <bits/stdc++.h>
#include "stdc++.h"
#include<stdlib.h>
#include<time.h>

// semilla para generar números aleatorios
int seed = 0;

// contador de evaluaciones
int cont_eval = 0;

using namespace std;

#define sz(lista) (int)lista.size()


// Estructura de datos para almacenar los datos de cada instancia
struct instancia {
    int n; // cantidad de puntos de control
    int m; // cantidad de rutas(equipos) a generar
    float tmax; // tiempo límite
    vector<float> x; // coordenadas x de los puntos de control
    vector<float> y; // coordenadas y de los puntos de control
    vector<int> s; // puntaje de cada punto de control (s_i) i = 1, ..., n};
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
    float distancia = sqrt(pow(nodo1.x-nodo2.x, 2) + pow(nodo1.y-nodo2.y, 2));
    return distancia;
}


// Función para calcular la tiempos total de una ruta
float calculateTotalTravelTime(std::vector<nodo> waypoints){
    float total_time = 0;
    for (int i = 1; i < sz(waypoints); i++){
        total_time += calculateTravelTime(waypoints[i], waypoints[i-1]);
    }
    return total_time;
}

// Función para calcular el puntaje total de una ruta
int calculateTotalScore(std::vector<nodo> waypoints){
    int total_score = 0;
    for (int i = 0; i < sz(waypoints); i++){
        total_score += waypoints[i].s;
    }
    return total_score;
}


// Función de Evaluación (calcular el puntaje total de todas las rutas 
// y penalizar si se excede el tiempo límite)
float evaluate(std::vector<ruta> rutas, instancia inst){
    float total_score = 0;
    for (int i = 0; i < sz(rutas); i++){
        total_score += calculateTotalScore(rutas[i].waypoints);
    }

    for (int i = 0; i < sz(rutas); i++){
        if (calculateTotalTravelTime(rutas[i].waypoints) > inst.tmax){
            total_score -= 1000000;
        }
    }

    cont_eval++;

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
    for (int i = 1; i < sz(nodos_iniciales)-1; i++){
        nodos_disponibles.push_back(nodos_iniciales[i]);
    }

    // Recorremos cada nodo 
    for (int i = 1; i < sz(nodos_iniciales)-1; i++){
        // Recorremos cada ruta
        for (int j = 0;j<inst.m;j++){
            int puntaje_max = 0;
            int indice_max = 0;
            float distancia_max = 0;
            // Recorremos cada nodo disponible
            for (int k = 0; k < sz(nodos_disponibles); k++){

                // Definimos la posible solución
                std::vector<nodo> posible_solucion = rutas[j].waypoints;

                // Agregamos el nodo disponible al final de la ruta
                posible_solucion.push_back(nodos_disponibles[k]);

                // Distancia entre el último nodo de la ruta y el penúltimo
                float distancia = calculateTravelTime(posible_solucion[sz(posible_solucion)-1], posible_solucion[sz(posible_solucion)-2]);
                

                // Agregamos el nodo final (n) al final de la ruta
                posible_solucion.push_back(nodos_iniciales[sz(nodos_iniciales)-1]);

                // Evaluamos la posible solución
                float puntaje = evaluate({{posible_solucion, 0}}, inst);

                // Si la distancia es menor a la distancia máxima y el puntaje es mayor a 0
                // actualizamos el puntaje máximo y el índice máximo
                if (distancia < distancia_max && puntaje > 0){
                    puntaje_max = puntaje;
                    indice_max = k;
                    distancia_max = distancia;
                }
                // Si la distancia es mayor a la distancia máxima y el puntaje es mayor a 0
                // actualizamos el puntaje máximo y el índice máximo
                else if (distancia > distancia_max && puntaje > 0){
                    puntaje_max = puntaje;
                    indice_max = k;
                    distancia_max = distancia;
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
    for (int i = 0; i < sz(rutas); i++){
        if (rutas[i].waypoints[sz(rutas[i].waypoints)-1].id != nodos_iniciales[sz(nodos_iniciales)-1].id){
            rutas[i].waypoints.push_back(nodos_iniciales[sz(nodos_iniciales)-1]);
        }
    }
    return rutas;
}


//Swap con nodos no ocupados aleatoriamiente
std::vector<ruta>swap(instancia inst, std::vector<nodo> nodos_iniciales, std::vector<ruta> rutas){
   
    // Creamos un vector de nodos disponibles
    std::vector<nodo> nodos_disponibles;
    for (int i = 0; i < sz(rutas); i++){
        for (int j = 0; j < sz(rutas[i].waypoints); j++){
            for (int k = 0; k < sz(nodos_iniciales); k++){
                if (rutas[i].waypoints[j].id == nodos_iniciales[k].id){
                    nodos_iniciales.erase(nodos_iniciales.begin()+k);
                }
            }
        }
    }

    srand(time(NULL));

    bool flag = true;
    while(flag){
    
        // Elegimos un nodo aleatorio de los disponibles
        int id_nodo = rand() % sz(nodos_iniciales);

        // Elegimos una ruta aleatoria
        int id_ruta = rand() % sz(rutas);

        // Definimos la posible solución
        std::vector<nodo> posible_solucion = rutas[id_ruta].waypoints;

        // Elegimos un waypoint aleatorio de la ruta que no sea el inicial ni el final
        int id_waypoint = rand() % (sz(posible_solucion)-2) + 1;

        // Agregamos el nodo en el waypoint elegido
        posible_solucion.insert(posible_solucion.begin()+id_waypoint, nodos_iniciales[id_nodo]);

        // Eliminamos el nodo que estaba en el waypoint+1
        posible_solucion.erase(posible_solucion.begin()+id_waypoint+1);

        // Revisamos si la solución es factible
        float puntaje = evaluate({{posible_solucion, 0}}, inst);

        if (puntaje > 0){
            rutas[id_ruta].waypoints = posible_solucion;
            rutas[id_ruta].puntaje = calculateTotalScore(posible_solucion);
            nodos_iniciales.erase(nodos_iniciales.begin()+id_nodo);
            return rutas;
        }
    }
    return rutas;
}

// Función que define la función de enfriamiento
float func_enfriamiento(float temp){
    return temp*0.9;
}

// Función que define si se acepta o no una solución
bool accept(float delta, float temp){
    if (delta > 0){
        delta = delta*(-1);
    }
    float prob = exp(delta/temp);
    float random = (float)rand()/(float)RAND_MAX;
    if (random < prob){
        return true;
    }
    return false;
}


// Función para ejecutar un algoritmo de Simulated Annealing
std::vector<vector<ruta>> simulated_annealing(instancia inst, std::vector<nodo> nodos_iniciales, std::vector<ruta> rutas, float temp0, int n_iter){
    float temp = temp0;
    float delta = 0;
    float puntaje_actual = evaluate(rutas, inst);
    float puntaje_nuevo = 0;
    std::vector<ruta> best_rutas = rutas;
    std::vector<ruta> rutas_nuevas;
    // Iteramos hasta que llegar al número de iteraciones
    for (int i = 0; i < n_iter; i++){
        // Ejecutamos el swap
        rutas_nuevas = swap(inst, nodos_iniciales, rutas);
        // Evaluamos la nueva solución
        puntaje_nuevo = evaluate(rutas_nuevas, inst);
        // Calculamos el delta
        delta = puntaje_nuevo - puntaje_actual;
        // Si el puntaje actual es mayor a 0
        if (puntaje_actual > 0){
            // Calculamos la probabilidad de aceptar la solución
            if (accept(delta, temp)){
                // Si la solución es aceptada, actualizamos la solución actual
                rutas = rutas_nuevas;
                puntaje_actual = puntaje_nuevo;
                // Si la solución actual es mejor que la mejor solución, actualizamos la mejor solución
                if (puntaje_actual > evaluate(best_rutas, inst)){
                    best_rutas = rutas;
                }
            }
        }
        // Cambiamos la temperatura
        temp = func_enfriamiento(temp);
    }
    return {best_rutas, rutas_nuevas};
}

int main(){
    
    string nombre_ruta = "instancias/Set_21/p2.2.a.txt";

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
    for (int i = 0; i < sz(nodos_iniciales); i++){
        printf("id: %d, x: %f, y: %f, s: %d\n", nodos_iniciales[i].id, nodos_iniciales[i].x, nodos_iniciales[i].y, nodos_iniciales[i].s);
    }
    printf("---------------------------------------------------------------\n");
    printf("Solución inicial (Greedy):\n");
    printf("---------------------------------------------------------------\n");
    std::vector<ruta> rutas = greedy(inst, nodos_iniciales);
    float puntaje_total = 0;
    for (int i = 0; i < sz(rutas); i++){
        printf("RUTA %d\n", i+1);
        printf("Waypoints:\n");
        printf("    id: %d, x: %f, y: %f, s: %d, t: -- \n", rutas[i].waypoints[0].id, rutas[i].waypoints[0].x, rutas[i].waypoints[0].y, rutas[i].waypoints[0].s);
        for (int j = 1; j < sz(rutas[i].waypoints); j++){
            printf("    id: %d, x: %f, y: %f, s: %d, t: %f\n", rutas[i].waypoints[j].id, rutas[i].waypoints[j].x, rutas[i].waypoints[j].y, rutas[i].waypoints[j].s, calculateTravelTime(rutas[i].waypoints[j], rutas[i].waypoints[j-1]));
        }
        printf("Puntaje: %d\n", rutas[i].puntaje);
        printf("Tiempo: %f\n", calculateTotalTravelTime(rutas[i].waypoints));
        puntaje_total += rutas[i].puntaje;
    printf("---------------------------------------------------------------\n");
    }
    printf("    Puntaje total: %d\n", (int)round(puntaje_total));
    printf("---------------------------------------------------------------\n");

    printf("Simulated Annealing ...\n");
    printf("---------------------------------------------------------------\n");

    // Definimos la temperatura inicial respecto a la cantidad de nodos
    float temp0_inst = inst.n*10;

    // Definimos la temperatura inicial respecto a la cantidad de rutas
    float temp0_rutas = inst.m*10;

    // Definimos la temperatura inicial respecto al tiempo límite
    float temp0_tmax = inst.tmax*10;


    int n_iter = 1000;

    std::vector<vector<ruta>> rutas_inst = simulated_annealing(inst, nodos_iniciales, rutas, temp0_inst, n_iter);
    std::vector<vector<ruta>> rutas_rutas = simulated_annealing(inst, nodos_iniciales, rutas, temp0_rutas, n_iter);
    std::vector<vector<ruta>> rutas_tmax = simulated_annealing(inst, nodos_iniciales, rutas, temp0_tmax, n_iter);

    printf("Puntajes Finales (Simulated Annealing):\n");

    printf("---------------------------------------------------------------\n");

    printf("Temperatura inicial(nodos): %f\n", temp0_inst);
    float puntaje_best_inst = evaluate(rutas_inst[0], inst);
    float puntaje_final_inst = evaluate(rutas_inst[1], inst);
    printf("    Puntaje total Best: %f\n", puntaje_best_inst);
    printf("    Puntaje total Final: %f\n", puntaje_final_inst);

    printf("---------------------------------------------------------------\n");

    printf("Temperatura inicial(rutas): %f\n", temp0_rutas);
    float puntaje_best_rutas = evaluate(rutas_rutas[0], inst);
    float puntaje_final_rutas = evaluate(rutas_rutas[1], inst);
    printf("    Puntaje total Best: %f\n", puntaje_best_rutas);
    printf("    Puntaje total Final: %f\n", puntaje_final_rutas);

    printf("---------------------------------------------------------------\n");

    printf("Temperatura inicial(t_max): %f\n", temp0_tmax);
    float puntaje_best_tmax = evaluate(rutas_tmax[0], inst);
    float puntaje_final_tmax = evaluate(rutas_tmax[1], inst);
    printf("    Puntaje total Best: %f\n", puntaje_best_tmax);
    printf("    Puntaje total Final: %f\n", puntaje_final_tmax);
    
    printf("---------------------------------------------------------------\n");

    ofstream archivo_solucion("solution.txt");

    float puntaje_best = puntaje_best_inst;
    std::vector<ruta> rutas_best = rutas_inst[0];

    if (puntaje_best_rutas > puntaje_best){
        puntaje_best = puntaje_best_rutas;
        rutas_best = rutas_rutas[0];
    }

    if (puntaje_best_tmax > puntaje_best){
        puntaje_best = puntaje_best_tmax;
        rutas_best = rutas_tmax[0];
    }
    
    archivo_solucion << puntaje_best << endl;

    for (int i = 0; i < sz(rutas_best); i++){
        archivo_solucion << rutas_best[i].puntaje << "\t";
        for (int j = 0; j < sz(rutas_best[i].waypoints); j++){
            archivo_solucion << rutas_best[i].waypoints[j].id << "\t";
        }
        archivo_solucion << endl;
    }
    
    archivo_solucion.close();

    printf("Solución Final:\n");
    printf("---------------------------------------------------------------\n");
    printf("    Puntaje total: %f\n", puntaje_best);
    printf("    Numero de evaluaciones: %d\n", cont_eval);
    printf("---------------------------------------------------------------\n");
    
    return 0;
}