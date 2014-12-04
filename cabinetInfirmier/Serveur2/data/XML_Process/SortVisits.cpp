#include "SortVisits.h"
#include <iostream>
#include <string.h>
#include <libxml/xmlmemory.h>
#include <libxml/debugXML.h>
#include <libxml/HTMLtree.h>
#include <libxml/xmlIO.h>
#include <libxml/DOCBparser.h>
#include <libxml/xinclude.h>
#include <libxml/catalog.h>
#include <libxslt/xslt.h>
#include <libxslt/xsltInternals.h>
#include <libxslt/transform.h>
#include <libxslt/xsltutils.h>
#include <curl/curl.h>
#include <stdlib.h>


#include "Kruskal.h"
#include "Adresse.h"
#include "FromXMLToGoogleMapHTTPRequest.h"
#include "FromGoogleMapXMLToDistanceTable.h"
// Constructeur
SortVisits::SortVisits() {
    
}

SortVisits::~SortVisits() {
    
}

size_t SortVisits::WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

void SortVisits::processDistanceMatrix(char * inputFileName, char * outputFileName, int id , std::string request) {
    
    std::vector<std::string> * adresses;
    std::vector< std::vector<int> > * distances;
    FromGoogleMapXMLToDistanceTable googleMapParser;
    
    
    curl = curl_easy_init();
    if(curl){
        std::cout << (prefix + request).c_str() << std::endl;
        curl_easy_setopt(curl, CURLOPT_URL, (prefix + request).c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl);
        
        if(res != CURLE_OK){
            fprintf(stderr, "curl_easy_perform() failed : %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        
        
        f = fopen("../reponseGoogle.xml", "w");
        if (f == NULL)
        {
            printf("Error opening file!\n");
            exit(1);
        }
        
        /* print some text */
        fprintf(f, readBuffer.c_str());
        
        fclose(f);
        
        googleMapParser.parseDocument("../reponseGoogle.xml");
        std::cout << std::endl;
        std::cout << std::endl;
        adresses = googleMapParser.getAdresses();
        distances = googleMapParser.getDistances();
        
        std::cout << "     | ";
        for(unsigned int c = 0; c < adresses->size(); c++) {
            std::string spacesBefore = "";
            if (c < 10)
                spacesBefore = "   ";
            else if (c < 100)
                spacesBefore = "  ";
            else if (c < 1000)
                spacesBefore = " ";
            else
                spacesBefore = "";
            
            std::cout << spacesBefore << c <<  " | ";
        }
        std::cout << std::endl << "  --------------------------------" << std::endl;
        
        for (unsigned int ligne = 0; ligne < adresses->size(); ligne++)
        {
            std::cout << "   " << ligne << " | ";
            for (unsigned int colonne = 0; colonne < adresses->size(); colonne++)
            {
                int value = (distances->at(ligne)).at(colonne);
                std::string spacesBefore = "";
                if (value < 10)
                    spacesBefore = "   ";
                else if (value < 100)
                    spacesBefore = "  ";
                else if (value < 1000)
                    spacesBefore = " ";
                else
                    spacesBefore = "";
                
                std::cout << spacesBefore << value << " | ";
            }
            std::cout << std::endl << "  --------------------------------" << std::endl;
        }
        
        
        // Ici, il faut appeler la fonction développée en RO
        //ParcoursPPrefixe(*distances);
        //////// modifié par moi
        
        
        
        std::string inputStd(inputFileName);
        std::string tmpFileName = inputStd.substr(0, inputStd.find_last_of("."));
        tmpFileName += "-sorted.xml";
        
        char * tmp = &tmpFileName[0];
        
        modifyFile(inputFileName, adresses, tmpFileName.c_str());
        saveXHTMLFile(tmp, outputFileName, id);
        
    }
}

std::string SortVisits::getPatientNodeAdresse(xmlpp::Node * adresseNode) {
    Adresse adresse;
    xmlpp::Node * node;
    xmlpp::Element * element;
    
    xmlpp::Node::PrefixNsMap nsMap;
    nsMap["cab"] = "http://www.ujf-grenoble.fr/l3miage/cabinet";
    
    // On récupère le numéro s'il existe
    if (adresseNode->find("cab:numéro", nsMap).size() > 0) {
        node = adresseNode->find("cab:numéro", nsMap).at(0);
        element = dynamic_cast <xmlpp::Element *> (node);
        if (element != NULL) {
            std::string numero = element->get_child_text()->get_content();
            adresse.setNumero(numero);
        }
    }
    
    // On récupère la rue
    node = adresseNode->find("cab:rue", nsMap).at(0);
    element = dynamic_cast <xmlpp::Element *> (node);
    if (element != NULL) {
        std::string rue = element->get_child_text()->get_content();
        adresse.setRue(rue);
    }
    
    // On récupère la ville
    node = adresseNode->find("cab:ville", nsMap).at(0);
    element = dynamic_cast <xmlpp::Element *> (node);
    if (element != NULL) {
        std::string rue = element->get_child_text()->get_content();
        adresse.setVille(rue);
    }
    
    // On récupère le code postal
    node = adresseNode->find("cab:codePostal", nsMap).at(0);
    element = dynamic_cast <xmlpp::Element *> (node);
    if (element != NULL) {
        std::string code = element->get_child_text()->get_content();
        adresse.setCodePostal(code);
    }
    
    return adresse.getGoogleAnswerAdress();
}
/// Faire le job...
void SortVisits::modifyFile(const char * inputFilename, std::vector<std::string> * adresses, const char * outputFilename) {
    std::cout << " Modifying xml file..." << std::endl;
    xmlpp::DomParser parser;
    parser.parse_file(inputFilename);
    if(parser)
    {
        //Walk the tree:
        xmlpp::Document * doc = parser.get_document();
        const xmlpp::Node* node = doc->get_root_node(); //deleted by DomParser.
        
        // Lorsque l'on a un namespace par défaut, il faut impérativement dans libxml (et libxml++)
        //   déclarer un préfixe pour ce namespace.
        xmlpp::Node::PrefixNsMap nsMap;
        nsMap["cab"] = "http://www.ujf-grenoble.fr/l3miage/cabinet";
        
        // --------------- On ordonne les patients selon leur adresse
        // On recherche et stocke tous les patients dans une map avec pour
        // clé leur adresse
        std::map<std::string, xmlpp::Element *> patientsAdresses;
        // On doit construire les adresse de la même manière que lors du parsing
        xmlpp::Node * patientsNode = node->find("//cab:patients", nsMap).at(0);
        xmlpp::NodeSet allPatients = patientsNode->find("cab:patient", nsMap);
        for(xmlpp::NodeSet::iterator iter = allPatients.begin(); iter != allPatients.end(); ++iter)
        {
            xmlpp::Element * patient = dynamic_cast<xmlpp::Element *>(*iter);
            // Récupérer l'adresse du patient en question
            xmlpp::Node * adresseNode = patient->find("cab:adresse", nsMap).at(0);
            std::string adresse = getPatientNodeAdresse(adresseNode);
            std::cout << adresse  << std::endl;
            xmlpp::Document * newDoc = new xmlpp::Document();
            patientsAdresses[adresse] = newDoc->create_root_node_by_import(patient);
            patientsNode->remove_child(patient);              
        }
        
        // Maintenant que tous les patients ont été supprimés du document, il faut les
        //  remettre dans le bon ordre
        
        // On parcourt les adresses d'entrées, on les cherche dans la map, si on les trouve, on les remets
        std::vector<std::string>::iterator it;
        for (it = adresses->begin(); it != adresses->end(); it++) {
            std::string sortedAdresse = (*it);
            xmlpp::Element * element = findAdresseInMap(sortedAdresse, patientsAdresses);
            if (element != NULL) {
                std::cout << "Child found !!, element name: " << std::string(element->get_name()) << std::endl;
                patientsNode->import_node(element);
            }
        }
        // Serialize the output file
        doc->write_to_file(outputFilename, "UTF-8");
    }
}


xmlpp::Element * SortVisits::findAdresseInMap(std::string sortedAdresse, std::map<std::string, xmlpp::Element *> map) {
    std::string sortedLower = sortedAdresse;
    std::transform(sortedLower.begin(), sortedLower.end(), sortedLower.begin(), ::tolower);
    std::cout << std::endl << "Searching for adresse: " << sortedLower << std::endl;
    
    xmlpp::Element * element = NULL;
    std::map<std::string, xmlpp::Element *>::iterator it = map.begin();
    while ((it != map.end()) && (element == NULL)) {
        std::string mapLower = (it->first);
        std::transform(mapLower.begin(), mapLower.end(), mapLower.begin(), ::tolower);
        
        std::cout << "it adresse:     " << mapLower << std::endl;
        
        if (sortedLower.find(mapLower) != std::string::npos) {
            element = it->second;
            std::cout << "Found !!" << std::endl;
        }
        it++;
    }    
    
    return element;
}

//See http://libxmlplusplus.sourceforge.net/docs/manual/html/ar01s02.html
void SortVisits::saveXHTMLFile(char * inputXMLFile, char * outputXHTMLFile, int id)
{
    FILE *outFile = NULL;
    //    const xmlChar * xsltFile = (const xmlChar *) ("data/ex.xsl");
    const xmlChar * xmlFile  = (const xmlChar *) (inputXMLFile);
    const char *params[16 + 1];
    
    int nbparams = 0;
    xsltStylesheetPtr cur = NULL;
    xmlDocPtr doc, res;
    
    char * nurseId = new char[4];
    sprintf(nurseId, "00%d", id);
    
    std::cout <<"Save XHTML File: input " << inputXMLFile << ", output: " << outputXHTMLFile << " with nurseId: " << nurseId << std::endl;
    
    params[0] = "destinedId";
    params[1] = nurseId;
    params[2] = NULL;
    
    xmlSubstituteEntitiesDefault(1);
    xmlLoadExtDtdDefaultValue = 1;
    cur = xsltParseStylesheetFile((const xmlChar *) ("../infosInfirmiers.xsl"));
    
    doc = xmlParseFile(inputXMLFile);
    
    res = xsltApplyStylesheet(cur, doc, params);
    outFile=(fopen(outputXHTMLFile,"w"));
    if(outFile==NULL){
        printf("Error!");
        exit(1);
    }
    xsltSaveResultToFile(outFile, res, cur);
    
    fclose(outFile);
    
    xsltFreeStylesheet(cur);
    xmlFreeDoc(res);
    xmlFreeDoc(doc);
    
    xsltCleanupGlobals();
    xmlCleanupParser();
    
    
}

void SortVisits::ParcoursPPrefixe(std::vector< std::vector<int> > G) {
    
    int i=0;
    /*
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
     
     
     */
    Kruskal kruskal(G);
    kruskal.arbre_min(); // calcul de l'arbre min, qui est alors stocké dans kruskal.acv (matrice d'adjacence)
    
    for (i = 0; i < G.size(); i++)
        for (int j = 0; j < i; j++)
            if (kruskal.acv[i][j] != 0)
                std::cout << i << " - " << j <<std::endl;
        
        
        std::cout << "Parcours en profondeur prefixe "<< std::endl;
        
	
        i=0;
        while ( i < G.size() ){
            DFS dfs(kruskal.acv); //On travaille sur l'abre min calculé plus haut par l'algo de Kruskall
            
            if(dfs.find(0,i) && dfs.Parcours.size()==G.size())
            {
                dfs.Parcours.push_back(0); // retour sommet initial
                for (int j=0; j<dfs.Parcours.size(); j++)
                {
                    std::cout << "Sommet : " << G[i][j]  << std::endl;
                }
            }
            //dfs.Parcours[j] + 
            i++;
        }
        
}