#include <bits/stdc++.h>
#include <time.h>
#include "Tools.hpp"

int main(int argc, char const *argv[]){
    if (argc == 1 || argc > 2){
        cout << "Ingrese el nombre de un solo fichero. " << endl;
        exit(-1);
    }
    
    string name  = argv[1];
    string name1 = name.substr(8,8);

    PGM Image(name);

    int opcion = 0;
    cout << " ******* Ingrese opción ******** " << endl;
    cout << "[0] Envolvente de figura con ID."  << endl;
    cout << "[1] Envolvente de cada figura."    << endl;
    cout << "[2] Envolvente de toda la Imagen." << endl;
    cout << "Opción: ";
    cin  >> opcion;
    
    Image.GetConvexSet();

    if (opcion == 0){
        int id = 0;
        cout << "ID de figura: "; cin  >> id;

        if (id < 1 || id > Image.convex) {
            cout << "No existe conjunto " << id << "." << endl;
            exit(-1);
        }
        
        clock_t start = clock();
        Image.ConvexHull_Single(id);
                
        clock_t end = clock();
        double cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC; 
        printf("Tiempo de Ejecución : %lf segundos.\n",cpu_time_used);
        Image.WritePGM("Out/result_" + name1);
    }

    if (opcion == 1){
        clock_t start = clock();
        Image.ConvexHull_Figures();
  
        clock_t end = clock();
        double cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC; 
        printf("Tiempo de Ejecución : %lf segundos.\n",cpu_time_used);
        Image.WritePGM("Out/result_" + name1);

    }
    if (opcion == 2){
        clock_t start = clock();
        Image.ConvexHull_Full();

        clock_t end = clock();
        double cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC; 
        printf("Tiempo de Ejecución : %lf segundos.\n",cpu_time_used);
        Image.WritePGM("Out/result_" + name1);
    }
    else{
        exit(0);
    }

    


    return 0;
}