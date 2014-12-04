#include "Kruskal.h"
#include <iostream>
#include <vector>

using namespace std;


int main()
{
    int i=0;
    vector<vector<int> > G(9);
    for (int i = 0; i < G.size(); i++)
        G[i].assign(G.size(), 0);

    G[0][1] = G[1][0] = 2;
    G[0][2] = G[2][0] = 1;
    G[0][7] = G[7][0] = 1;
    G[1][2] = G[2][1] = 2;
    G[1][3] = G[3][1] = 3;
    G[2][3] = G[3][2] = 2;
    G[2][4] = G[4][2] = 1;
    G[2][7] = G[7][2] = 1;
    G[3][4] = G[4][3] = 1;
    G[3][5] = G[5][3] = 2;
    G[4][5] = G[5][4] = 1;
    G[4][6] = G[6][4] = 1;
    G[5][6] = G[6][5] = 1;
    G[6][7] = G[7][6] = 2;
    G[6][8] = G[8][6] = 2;
    G[7][8] = G[8][7] = 2;

    Kruskal kruskal(G);
    kruskal.arbre_min(); // calcul de l'arbre min, qui est alors stocké dans kruskal.acv (matrice d'adjacence)

    for (int i = 0; i < G.size(); i++)
        for (int j = 0; j < i; j++)
            if (kruskal.acv[i][j] != 0)
            cout << i << " - " << j <<endl;


                cout << "Parcours en profondeur prefixe "<< endl;

    while ( i <= G.size() ){
        DFS dfs(kruskal.acv); //On travaille sur l'abre min calculé plus haut par l'algo de Kruskall

        if(dfs.find(0,i) && dfs.Parcours.size()==G.size())
            {
                dfs.Parcours.push_back(0); // retour sommet initial
                    for (int j=0; j<dfs.Parcours.size(); j++)
                    {
                        cout << "Sommet : " << dfs.Parcours[j] << endl;
                    }
            }
        i++;
    }

    return 0;
}


