#include "LwSaxParser.h"


#include <string>

class Adresse;
class FromXMLToGoogleMapHTTPRequest : public LwSaxParser {

public:
    enum PossibleStates {START, PATIENT, ADRESSE, ETAGE, NUMERO, RUE, VILLE, CODEPOSTAL, VISITE, OTHER};
    
    /// Constructeur
    FromXMLToGoogleMapHTTPRequest(std::string id);
    
    /// Desctructeur
    ~FromXMLToGoogleMapHTTPRequest();
    
    std::string getGoogleMapHttpRequest();
    
protected:
    bool adresseCabinet;
    
    /// Attributs
    std::string request;
    
    /// id de l'infirmière
    std::string id;
    
    /// Etat courant
    PossibleStates state;
    
    /** Stockage des listes d'adresse. Sera utilisé pour former la requête GoogleMap.
      * Les adresses sont à stocker grâce à la méthode getGoogleAdresse() de la classe Adresse
      *    et doivent être séparées par le caractère |  (pipe)
      */
    std::vector<Adresse> listeAdresses;
    
    Adresse * adresseCourante;
    
    /** 
    * Méthodes ré-implemntées (surchargées) de la classe SaxParser
    * @{
    */
    
    // A compléter en recompiant la signature des méthodes à redéfinir
    
    /**
     * @}
     */
   
protected:
    // Cherche attributeName dans la liste des attributs et renvoie sa valeur
    // renvoie la chaîne de caractères vide si l'attribut n'est pas trouvé.
    std::string findAttribute(const AttributeList& attributeList, std::string attributeName);


};

