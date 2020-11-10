#ifndef TOOLS_HPP
#define TOOLS_HPP

using namespace std;

class PGM{
    private:
        int rows   = 0;
        int cols   = 0;
        vector<vector<int>>  Image;    // Guarda Imagen Original
        vector<vector<int>>  Figures;  // Enumero figuras
        vector<vector< int>> FiguresID;// Id y tamaño
        
        void FilteredPGM();            // Filtra pixeles grises
        void GetMaxMin();              // Escribe Max y Min en Figures

        // Conjuntos Conexos
        int CheckNeibors(vector <int> &indexed, int figure_index, queue<vector<int>> &myQueue);
        void CheckLocalNeibors( int i, int j, int figure_index, queue<vector<int>> &myQueue);
        
        // Envolvente Convexa
        int distance_compare(vector <int> &p1, vector <int> &p2,vector <int> &p3);
        int get_orientation(vector <int> &p1, vector <int> &p2, vector <int> &p3); 
        void PaintNeibors(int i, int j);
        void lines(vector<vector<int>> &Hull);
    public:
        int convex    = 0;
        int max_scale = 0;

        PGM(string filename);
        // Conjuntos Conexos
        void GetConvexSet();
        void PrintFigures();

        // Envolvente Convexa
        void ConvexHull_Single(int id);
        void ConvexHull_Figures();
        void ConvexHull_Full();

        void WritePGM(string filename);   // Escribe imagen original
        void WritePGM_MM(string filename);// Escribe Max y Min
};

#endif
