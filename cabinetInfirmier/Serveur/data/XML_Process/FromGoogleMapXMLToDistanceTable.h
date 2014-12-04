#include "LwSaxParser.h"
#include <vector>
#include "Adresse.h"

class FromGoogleMapXMLToDistanceTable : public LwSaxParser {
    
public:
    enum PossibleStates {START, ORIGIN_ADDRESS, ROW, ELEMENT, DISTANCE, VALUE, UNKNOWN};
    
    /// Constructeur
    FromGoogleMapXMLToDistanceTable();
    
    /// Desctructeur
    ~FromGoogleMapXMLToDistanceTable();

   void init();    
    /// Liste des adresses
   std:: vector<Adresse> getAdresses();
    
    /**
     * Distance en mètres entre les adresses
     * Le vecteur général est un vecteur de ligne.
     * Sur chaque ligne numéro i, il y a la distance entre l'adresses numéro i de getAdresses
     *  et chacune des adresses numéro j (à la colonne j).
     */
    std::vector< std::vector<int> > * getDistances();
    
protected:
   /** 
    * Méthodes ré-implemntées (surchargées) de la classe SaxParser
    * @{
    */
    
    // A compléter en recompiant la signature des méthodes à redéfinir
    
   /**
    * @}
    */
  
        
    /// Attributs
    
   /// Etat courant
   int state;
   
   std::string adresseCourante;
   /// Vecteur des adresses à remplir
   std::vector<Adresse> adresses;
   
    /// Matrice des distances à remplir
   std::vector<int> * ligne;
   std::vector< std::vector<int> > * distanceMatrix;
    
};
