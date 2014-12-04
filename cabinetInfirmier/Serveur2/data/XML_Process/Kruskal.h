#include <vector>
#include <queue>
#include <algorithm>

class DFS
// Class effectuant un parcours en profondeur et qui sert � savoir si un sommet cible a �t� trouv�
{
    const std::vector<std::vector<int> > &G;
    std::vector<bool> marque;

public:
    std::vector<int> Parcours; // Tableau Parcours pr�fix�

    DFS(const std::vector<std::vector<int> > &G) : G(G), marque(G.size(), false) {}

    bool find(int source, int cible)
    {
        marque[source] = true;
        Parcours.push_back(source);
        if (source == cible) return true;

        for (int i = 0; i < G.size(); i++)
            if (G[i][source] != 0 && marque[i] == false) // si il y a un sommet entre source et i et si i n'est pas marqu�
                if (find(i, cible))
                    return true; // on a trouv� cible
        return false; // on ne l'a pas trouv�
    }
};


class Kruskal
{
    const std::vector<std::vector<int> > &G;
    class Edge
    {
    public:
        int u, v, poids;
        Edge(int u, int v, int poids) : u(u), v(v), poids(poids) { }
        bool operator<(const Edge & e) const // utilis� pour trier le vector aretes dans arbre_min()
        {
            return poids < e.poids;
        }
    };

public:
    std::vector<std::vector<int> > acv; // matrice d'adjacence de l'arbre couvrant minimum

    Kruskal(const std::vector<std::vector<int> > &G) : acv(G.size()), G(G)
    {
        for (int i = 0; i < G.size(); i++)
            acv[i].assign(G.size(), 0);
    }

    bool arbre_min() // trouve l'arbre couvrant minimum et renvoit true si il existe, false sinon
    {
        int nEdges = 0; // nombre d'aretes deja ajoutees a l'arbre
        std::vector<Edge> aretes;

        for (int i = 0; i < G.size(); i++)
            for (int j = 0; j < i; j++)
                if (G[i][j] != 0)
                    aretes.push_back(Edge(i, j, G[i][j]));
        std::sort(aretes.begin(), aretes.end()); // on trie le tableau

        for (int i = 0; i < aretes.size(); i++)
        {
            Edge next = aretes[i]; // on essaie de rajouter next
            DFS dfs(acv);
            if (!dfs.find(next.u, next.v)) // si l'ajout de next ne donne pas de cycle (c'est � dire si next.v n'est pas joignable depuis next.u)
            {
                acv[next.u][next.v] = acv[next.v][next.u] = G[next.u][next.v]; // on ajoute l'arete a l'arbre couvrant
                if (++nEdges == G.size() - 1) // un arbre avec n sommets a n-1 aretes
                    return true;
            }
        }
        return false;
    }
};
