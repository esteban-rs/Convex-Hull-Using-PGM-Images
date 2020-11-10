#include <bits/stdc++.h>
#include "Tools.hpp"

PGM::PGM(string filename) {
    ifstream file(filename);

    if (file.is_open() == 0){
        cerr << "No existe el archivo." <<endl;
        exit(-1);
    }

    string line;              // Variable local
    /* **** C o m i e n z a  l e c t u r a **** */                 
    getline(file, line);
    if (line.compare("P2") != 0){
        cout << "Formato no válido." << endl;
    }

    getline(file, line);
    while (line[0] == '#'){  // Salto comentarios
        getline(file, line);
    }
    /* *********** A l o c a c i o n ********** */                 
    stringstream ss(line);  // Obtengo dimensiones
    getline(ss,line,' ');
    rows = stoi(line);      
    cols = stoi(line);
    Image.assign(rows, vector<int> (cols,0));
    file >> max_scale;
    /* ******* L e e  C o n t e n i d o ******* */                 
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            file >> Image[i][j];
        }
    }
    file.close();
    FilteredPGM();

}

void PGM::FilteredPGM(){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (Image[i][j] < max_scale/2){
                Image[i][j] = 0;
            }
            else {
                Image[i][j] = max_scale;   
            }
        }
    }
}

void PGM::WritePGM(string filename) {
    ofstream file(filename);
    // Comienza escritura
    file << "P2"<<endl;
    file << "# CREATED BY ESTEBANRS" << endl;
    file << rows << " " << cols << endl;
    file << max_scale << endl;

    // Escribe valores
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            file << Image[i][j] << endl;
        }
    }
    file.close();

    cout << "¡Hurra! Se escribió imagen en " << filename << endl;
}

void PGM::GetMaxMin(){
    int MAXID = FiguresID[0][0];
    int MINID = FiguresID[0][0];
    int MAX   = FiguresID[0][1];
    int MIN   = FiguresID[0][1];

    for (int i = 1; i < FiguresID.size(); i++){
        if (FiguresID[i][1] > MAX){
            MAXID = FiguresID[i][0];
            MAX   = FiguresID[i][1];
        }
        if (FiguresID[i][1] < MIN){
            MINID = FiguresID[i][0];
            MIN   = FiguresID[i][1];         
        }
    }

    for (int i = 0; i <rows; i++){
        for (int j = 0; j < cols; j++){
            if (Figures[i][j] == MAXID || Figures[i][j] == MINID){
                Figures[i][j] = max_scale;
            }
            else{
                Figures[i][j] = 0;
            }
        }
    }
}

void PGM::WritePGM_MM(string filename) {
    //GetMaxMin();
    ofstream file(filename);
    // Comienza escritura
    file << "P2"<<endl;
    file << "# CREATED BY ESTEBANRS" << endl;
    file << rows << " " << cols << endl;
    file << max_scale << endl;

    // Escribe valores
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            file << Figures[i][j] << endl;
        }
    }
    file.close();

    cout << "¡Hurra! Se escribió imagen en " << filename << endl;
}

void PGM::GetConvexSet(){
    // Asigno memoria a matrix Image
    Figures.assign(rows, vector<int> (cols,0));

    queue<vector<int>> MyQueue;// Cola para pixeles de figura

    vector <int> position;     // Posicion Pixel Figura
    position.assign(2,0);

    vector <int> FindSet;      // Figura Encontrada
    FindSet.assign(2,0);       // [0] id [1] size

    int figure_id   = 1;
    int figure_size = 0;
    int tmp = 0;
    // Recorro Imagen
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            // Reviso si hay figura y no se ha marcado
            if (Image[i][j] != 0 && Figures[i][j] == 0) {
                // Agrego punto a cola
                position[0] = i;
                position[1] = j;
                MyQueue.push(position);
                // Actualizo tamaño
                figure_size++;
                // Marco Pixel con indice
                Figures[i][j] = figure_id;
            
                while (!MyQueue.empty()){
                    position = MyQueue.front();
                    MyQueue.pop();
                    figure_size += CheckNeibors(position,figure_id,MyQueue);
                }
                // Guardo indice y tamaño
                FindSet[0] = figure_id;
                FindSet[1] = figure_size;
                FiguresID.push_back(FindSet);
                // Actualizo indice y reinicio tamaño
                figure_id++; 
                figure_size = 0;
            }           
        } //EndFor j
    } // EndFor i
    position.clear();
    FindSet.clear();
    convex = Figures.size();
}

void PGM::PrintFigures(){
    cout << "** F i g u r e s **" << endl;
    for (int i = 0; i < FiguresID.size(); i++){
        cout << "[" << FiguresID[i][0] << "] " << FiguresID[i][1]<< endl;
    }
}

void PGM::CheckLocalNeibors( int i, int j, int id, queue<vector<int>> &myQueue){
    // Vector para agregar elementos a cola
    vector <int> tmp;
    tmp.assign(2,0);
    // Guardo posicion
    tmp[0] = i;
    tmp[1] = j;
    // Agrego a cola
    myQueue.push(tmp);
    // Marco pixel
    Figures[i][j] = id;
    // Libero posicion temporal
    tmp.clear();
}

int PGM::CheckNeibors(vector <int> &indexed, int figure_id, queue<vector<int>> &myQueue){
    int i = indexed[0];
    int j = indexed[1];
    int neibors = 0;

    // Reviso todos los casos
    if (i >= 0 && j -1 >= 0){
        if (Image[i][j-1] != 0 && Figures[i][j-1] == 0){
            CheckLocalNeibors(i,j-1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i -1 >= 0 && j >= 0){
        if (Image[i-1][j] != 0 && Figures[i-1][j] == 0){
            CheckLocalNeibors(i-1,j,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i - 1 >= 0 && j - 1 >= 0){
        if (Image[i-1][j-1] != 0 && Figures[i-1][j-1] == 0){
            CheckLocalNeibors(i-1,j-1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i - 1 >= 0 && j + 1 < cols){
        if (Image[i -1][j+1] != 0 && Figures[i-1][j+1] == 0){
            CheckLocalNeibors(i-1,j+1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i + 1 < rows && j - 1 >= 0){
        if (Image[i+1][j-1] != 0 && Figures[i+1][j-1] == 0){
            CheckLocalNeibors(i+1,j-1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i >= 0 && j + 1 < cols){
        if (Image[i][j+1] != 0 && Figures[i][j+1] == 0){
            CheckLocalNeibors(i,j+1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i + 1 < rows && j >= 0){
        if (Image[i+1][j] != 0 && Figures[i+1][j] == 0){
            CheckLocalNeibors(i+1,j,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }

    if (i + 1 < rows && j + 1 < cols){
        if (Image[i+1][j+1] != 0 && Figures[i+1][j+1] == 0){
            CheckLocalNeibors(i+1,j+1,figure_id,myQueue);
            // Agrego pixel a tamaño
            neibors++;
        }
    }
    return neibors;
}

int PGM::distance_compare(vector <int> &p1, vector <int> &p2,vector <int> &p3){
    // Regresa como resultado si p3 está más lejos de p2
    double value1 = sqrt((p1[0]-p2[0])*(p1[0]-p2[0])+(p1[1]-p2[1])*(p1[1]-p2[1]));
    double value2 = sqrt((p1[0]-p3[0])*(p1[0]-p3[0])+(p1[1]-p3[1])*(p1[1]-p3[1]));
    if (value1 < value2){
        return 1;
    }
    else{
        return 0;
    }
}

int PGM::get_orientation(vector <int> &p1, vector <int> &p2, vector <int> &p3){
    // Regresa la orientacion de los puntos dados
    int sigma = (p3[0] - p2[0])*(p2[1] - p1[1]);
    int tau   = (p2[0] - p1[0])*(p3[1] - p2[1]);
    
    if ((sigma - tau) == 0){   
        return 0;             // [0] Colineales
    }
    else if (sigma - tau > 0){ 
        return 1;             // [1] Horario
    }
    else{                      
        return 2;             // [2] Antihorario
    }
}

void PGM::ConvexHull_Single(int id){
    // Recibe índice de figura previamente calculada en GetConvexSet()
    
    // Termino si tengo línea o punto como figura
    if (FiguresID[id-1][1] < 3){
        cout << "No Convex Hull Found."<< endl;
        return;
    }

    vector< vector <int>> figure; // Puntos de Figura
    vector<int> pixel;            // Punto temporal
    pixel.assign(2,0);

    // Agrego a lista si tiene etiqueta id
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (Figures[i][j] == id){
                pixel[0] = i;
                pixel[1] = j;
                figure.push_back(pixel);
            }
        }
    }
    /* Por la forma en la que se recorrió la matriz, 
    los puntos en figure están ordenados. */

    // Jarvis March
    vector< vector <int>> Hull; // Lista para puntos
    
    int p           = 0;  // Indice actual
    int q           = 0;  // Actualizacion
    int orientation = 2;

    while (true){
        // Agrego punto a Envolvente
        Hull.push_back(figure[p]);

        // Comienzo busqueda en el siguiente punto
        q = (p + 1)%figure.size();

        for (int i = 0; i < figure.size(); i++){
            orientation = get_orientation(figure[p], figure[i], figure[q]);
            
            if (orientation == 2){
                q = i;              // Actualizo si orientacion es antihoraria
            }
            if (orientation == 0){
                if (distance_compare(figure[p], figure[i], figure[q]) == 0){
                    q = i;          // Actualizo si son colineales y distancia es mayor
                }
            }
        }
        p = q;                      // Actualizo punto inicial
        
        if (p == 0) break;          // Termino si regreso a primer punto
    }
    figure.clear();
    pixel.clear();
    
    lines(Hull);

    cout << "* ***** F i g u r a ["   << id << "] ***** *" << endl;
    cout << "Tamaño de Envolvente : " << Hull.size()<< endl;
    
    for (int i = 0; i < Hull.size(); i++){
        cout<< "Punto: " << "["<<Hull[i][0] << "]" << "["<< Hull[i][1] << "] ";
        if (i > 0 && (i+1)%4 == 0) cout << endl;
    }
    cout << endl;
    
    Hull.clear();
}

void PGM::ConvexHull_Figures(){
    // Obtiene todas las envolventes convexas de todas las figuras
    for (int i = 1; i <= FiguresID.size(); i++){
        ConvexHull_Single(i);
    }
}

void PGM::ConvexHull_Full(){
    vector< vector <int>> figure; // Puntos de Figura
    vector<int> pixel;            // Punto temporal
    pixel.assign(2,0);

    // Agrego a lista si tiene etiqueta id
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (Image[i][j] == max_scale){
                pixel[0] = i;
                pixel[1] = j;
                figure.push_back(pixel);
            }
        }
    }
    /* Por la forma en la que se recorrió la matriz, 
    los puntos en figure están ordenados. */

    // Jarvis March
    vector< vector <int>> Hull; // Lista para puntos
    
    int p           = 0;        // Indice actual
    int q           = 0;        // Actualizacion
    int orientation = 2;

    while (true){
        // Agrego punto a Envolvente
        Hull.push_back(figure[p]);

        // Comienzo busqueda en el siguiente punto
        q = (p + 1)%figure.size();

        for (int i = 0; i < figure.size(); i++){
            orientation = get_orientation(figure[p], figure[i], figure[q]);
            
            if (orientation == 2){
                q = i;              // Actualizo si orientacion es antihoraria
            }
            if (orientation == 0){
                if (distance_compare(figure[p], figure[i], figure[q]) == 0){
                    q = i;          // Actualizo si son colineales y distancia es mayor
                }
            }
        }
        p = q;                      // Actualizo punto inicial
        
        if (p == 0) break;          // Termino si regreso a primer punto
    }
    figure.clear();
    pixel.clear();

    // Dibujo líneas
    lines(Hull);

    cout << "* ***** F i g u r a ***** *" << endl;
    cout << "Tamaño de Envolvente : " << Hull.size()<< endl;
    
    for (int i = 0; i < Hull.size(); i++){
        cout << "Punto: " << "["<<Hull[i][0] << "]" << "["<< Hull[i][1] << "] ";
        if (i > 0 && (i+1)%4 == 0) cout << endl;
    }
    cout << endl;

    Hull.clear();
}

void PGM::PaintNeibors(int i, int j){
    // Reviso cuatro Vecinos
    int color = max_scale/2;
    
    // Reviso todos los casos
    if (i >= 0 && j -1 >= 0){
        Image[i][j-1] = color;
    }
    if (i -1 >= 0 && j >= 0){
        Image[i-1][j] = color;
    }
    if (i >= 0 && j + 1 < cols){
        Image[i][j+1] = color;
    }
    if (i + 1 < rows && j >= 0){
        Image[i+1][j] = color;
    }
}

void PGM::lines(vector<vector<int>> &Hull) {
    // Guarda posición (x_1,y_1) y (x_2, y_2)
    int x_1 = 0;
    int y_1 = 0;
    int x_2 = 1;
    int y_2 = 1;
    
    float m   = 0;
    int pivot = 0;

    // Dibujo segmentos p1-p2, p2-p3, ..., pn-p1
    for (int i = 0; i < Hull.size(); i++){
        //tomo puntos p_i - p_{i+1}
        if (i != Hull.size()-1){
            x_1 = Hull[i][0];
            y_1 = Hull[i][1];
            x_2 = Hull[i+1][0];
            y_2 = Hull[i+1][1];
        }
        //Revisa Primer y último punto de Hull
        else {
            x_1 = Hull[0][0];
            y_1 = Hull[0][1];
            x_2 = Hull[Hull.size()-1][0];
            y_2 = Hull[Hull.size()-1][1];
        }
		// |x_2 - x_1 | > |y_2 - y_1 |
		if(abs(x_2 - x_1) > abs(y_2 - y_1)){
            // Pendiente indefinida
    	    if (x_1 == x_2){
                if(y_1 < y_2){
                    for (int y = y_1; y <= y_2; y++){
                        Image[x_1][y] = max_scale/2;
                        PaintNeibors(x_1,y);
                    }
                }
                // y_1 > y_2
		        else {
                    for (int y = y_2; y <= y_1; y++){
                        Image[x_1][y] = max_scale/2;
                        PaintNeibors(x_1,y);
                    }
                }
		    }		
            else if (x_1 < x_2){
                m     = (float) (y_2 - y_1)/(x_2 - x_1);

                for (int x = x_1; x <= x_2; x++){
                    pivot = (int) (m*(x-x_1) + y_1);
                        
                    Image[x][pivot] = max_scale/2;
                    PaintNeibors(x,pivot);
                }
            }
            else {
                m     = (float) (y_2 - y_1)/(x_2 - x_1);

                for (int x = x_2; x <= x_1; x++){
                    pivot = (int) (m*(x-x_1) + y_1);
                      
                    Image[x][pivot] = max_scale/2;
                    PaintNeibors(x,pivot);
                }
            }	
		}
		// |x_2 - x_1 | <= |y_2 - y_1 |
        else {
            // Pendiente indefinida
            if (y_1 == y_2){
                if(x_1 < x_2){
                    for (int x = x_1; x <= x_2; x++){
                        Image[x][y_1] = max_scale/2;
                        PaintNeibors(x,y_1);
                    }
                }
                // x_1 > x_2
                else {
                    for (int x = x_2; x <= x_1; x++){
                        Image[x][y_1] = max_scale/2;
                        PaintNeibors(x,y_1);
                    }
                }
            }		
            else if (y_1 < y_2){
                m     = (float) (x_2 - x_1)/(y_2 - y_1);

                for (int y = y_1; y < y_2; y++){
                    pivot = (int) (m*(y-y_1)+x_1);

                    Image[pivot][y]=max_scale/2;
                    PaintNeibors(pivot,y);
                }
            }
            // y_1 > y_2
            else {
                m     = (float) (x_2 - x_1)/(y_2 - y_1);

                for (int y = y_2; y < y_1; y++){
                    pivot = (int) (m*(y-y_1)+x_1);

                    Image[pivot][y]=max_scale/2;
                    PaintNeibors(pivot,y);
                }
            }		            
        }
    }
}
