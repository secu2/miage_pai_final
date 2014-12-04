#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <fstream>


#include "FromXMLToGoogleMapHTTPRequest.h"
#include "FromGoogleMapXMLToDistanceTable.h"
#include "SortVisits.h"

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}


int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cout << "Veuillez entrer le nom du fichier à analyser en paramètre de la commande : testParsers filename.xml id" << std::endl;
        std::cout << "Exemple: " << std::endl;
        std::cout << "> ./testParsers ../data/cabinetInfirmier.xml 001" << std::endl << std::endl;
        return 1;
    }
    char * filename = argv[1];
    char * id = argv[2];
    int idInt = 2;
    
    const char * charRequest;
    
    FromXMLToGoogleMapHTTPRequest dataBaseParser(id);
    FromGoogleMapXMLToDistanceTable googleMapParser;
    SortVisits sort;
    std::vector<std::string> * adresses;
    std::vector< std::vector<int> > * distances;
    
    std::string request;
    std::string prefix = "https://maps.googleapis.com/maps/api/distancematrix/xml?";
    
    CURL *curl;
    CURLcode res;
    
    std::string readBuffer;
    FILE *f;
    
  
      
	      dataBaseParser.parseDocument(filename);
		request = dataBaseParser.getGoogleMapHttpRequest();
		 charRequest = request.c_str();
		
		//strcpy (test2, test.c_str());
                //googleMapParser.parseDocument("https://maps.googleapis.com/maps/api/distancematrix/xml?origins=%7CGrenoble%2B38041%2B60%2BRue%20de%20la%20chimie%7CGrenoble%2B38031%2B46%2BAvenue%20Felix%20Viallet%7CLa%20Tronche%2B38700%2Brond-Point%20de%20la%20Croix%20de%20Vie&destinations=%7CGrenoble%2B38041%2B60%2BRue%20de%20la%20chimie%7CGrenoble%2B38031%2B46%2BAvenue%20Felix%20Viallet%7CLa%20Tronche%2B38700%2Brond-Point%20de%20la%20Croix%20de%20Vie");
		char name[13] = "test";
		strcat(name, id);
		strcat( name, ".html");
		printf("\n\n\n\n\nname = %s\n\n\n\n\n\n\n",name);
		sort.processDistanceMatrix(filename,name,atoi(id),request);
                
		/*
		 
                // écrire la liste des adresses:
                for (unsigned int i = 0; i < adresses->size(); i++)
                {
                    std::cout << "| " << i << " | " << adresses->at(i) << " | " << std::endl;
                }
                std::cout << std::endl;
                std::cout << std::endl;
                
                
                // écrire le tableau des distances avec les index
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
                
                break;
                */
		
		
                   
    
	
	return 0;
}
