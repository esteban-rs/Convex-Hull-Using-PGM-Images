#include <bits/stdc++.h>
#include <time.h>
#include "Tools.hpp"

int main(int argc, char const *argv[]){
    if (argc == 1 or argc >2){
        cout << "Ingrese el nombre de un solo fichero. " << endl;
        exit(-1);
    }
    
    string name  = argv[1];
    string name1 = name.substr(8,8);

    clock_t start = clock();
    PGM Image(name);
    Image.GetConvexSet();
    Image.PrintFigures();
    Image.WritePGM_MM("Out/result_" + name1);
    clock_t end = clock();
    double cpu_time_used = ((double)(end - start))/CLOCKS_PER_SEC; 
    printf ("Tiempo de Ejecución : %lf segundos.\n",cpu_time_used);
    return 0;
}