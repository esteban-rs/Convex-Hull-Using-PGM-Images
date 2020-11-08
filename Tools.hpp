#ifndef TOOLS_HPP
#define TOOLS_HPP

using namespace std;

class PGM{
    private:
        int rows = 0;
        int cols = 0;
        vector<vector<int>>  Image;    // Guarda Imagen Original
        vector<vector<int>>  Figures;  // Enumero figuras
        vector<vector< int>> FiguresID;// Id y tamaño
        
        void FilteredPGM();            // Filtra pixeles grises
        int CheckNeibors(vector <int> &indexed, int figure_index, queue<vector<int>> &myQueue);
        void CheckLocalNeibors( int i, int j, int figure_index, queue<vector<int>> &myQueue);
        void GetMaxMin();              // Escribe Max y Min en Figures
    public:
        int max_scale = 0;
        PGM(string filename);
        void GetConvexSet();
        void PrintFigures();
        void WritePGM(string filename); // Escribe imagen original
        void WritePGM_MM(string filename);// Escribe Max y Min
};



#endif
