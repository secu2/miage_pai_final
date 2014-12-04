#include <string>
#include <vector>
#include <map>
#include <curl/curl.h>
#include <stdlib.h>
/*
 * Classe qui contient un parser DOM pour modifier le fichier d'entrée
 *  en y ajoutant l'ordre des adresses à visiter
 */
namespace xmlpp {
    class Node;
    class Element;
}

class SortVisits {
    
public:
    /// Constructeur
    SortVisits();
    
    /// Desctructeur
    ~SortVisits();
    
    /// Méthode qui fait tout
    void processDistanceMatrix(char * inputFileName, char * outputFileName, int id,std::string request);
    
    /// Faire le job...
    void modifyFile(const char * inputFilename, std::vector<std::string> * adresses, const char * outputFilename);
    
    /// Faire du xslt
    void saveXHTMLFile(char * inputXMLFile, char * outputXHTMLFile, int id);
    
protected:
    std::string getPatientNodeAdresse(xmlpp::Node * adresseNode);
    xmlpp::Element * findAdresseInMap(std::string sortedAdresse, std::map<std::string, xmlpp::Element *> map);
    void ParcoursPPrefixe(std::vector< std::vector<int> >  G);
    static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp);
    
    std::string request;
    std::string prefix = "https://maps.googleapis.com/maps/api/distancematrix/xml?";
    
    CURL *curl;
    CURLcode res;
    
    std::string readBuffer;
    FILE *f;
};