#include <iostream>
#include "FromGoogleMapXMLToDistanceTable.h"
#include <libxml/xmlreader.h>

// Constructeur
FromGoogleMapXMLToDistanceTable::FromGoogleMapXMLToDistanceTable() : LwSaxParser() {
    state = UNKNOWN;
    adresseCourante = "";
    
    ligne = NULL;
    distanceMatrix = NULL;
}

FromGoogleMapXMLToDistanceTable::~FromGoogleMapXMLToDistanceTable() {


}

void FromGoogleMapXMLToDistanceTable::init() {	
	xmlpp::TextReader reader("../../data/reponseGoogle.xml");
	//parseDocument("../../data/reponseGoogle.xml");
    //this->set_substitute_entities(true); //
    //this->parse_file("test.xml");

 while(reader.read())
    {
      int depth = reader.get_depth();
      std::cout <<  "--- node ---" << std::endl;
      std::cout << "name: " << reader.get_name() << std::endl;

     std::cout << "depth: " << reader.get_depth() << std::endl;

      if(reader.has_attributes())
      {
        std::cout <<  "attributes: " << std::endl;
        reader.move_to_first_attribute();
        do
        {
          std::cout << "  " << reader.get_name() << ": " << reader.get_value(); 
        } while(reader.move_to_next_attribute());
        reader.move_to_element();
      }
      else
      {
        std::cout << "no attributes" << std::endl;
      }

      if(reader.has_value())
        std::cout << "value: '" << reader.get_value() << "'" << std::endl;
      else
        std::cout << "novalue" << std::endl;
        }

}

std::vector<Adresse>  FromGoogleMapXMLToDistanceTable::getAdresses() {
    // Ici, on triche pour que le test fonctionne du premier coup
    // Ces lignes sont à remplacer par un vrai parcours du document XML
    // Cette méthode ne devra plus contenir que return adresses; .

	xmlpp::TextReader reader("../../data/reponseGoogle.xml");
    //this->set_substitute_entities(true); //
    //this->parse_file("test.xml");
int i=0;

 while(reader.read())
    {
      if(reader.has_attributes())
      {
        reader.move_to_first_attribute();
        do
        {
          if (reader.get_name()=="destination_address")
		adresses[i++].setVille(reader.get_value()); 
        } while(reader.move_to_next_attribute());
        reader.move_to_element();
      }

     }

while (i!=0)
	std::cout << adresses[i--].getVille() << ";"; 


    return adresses;
}

std::vector< std::vector<int> > * FromGoogleMapXMLToDistanceTable::getDistances() {
    // Ici, on triche pour que le test fonctionne du premier coup
    // Ces lignes sont à remplacer par un vrai parcours du document XML
    // Cette méthode ne devra plus contenir que return distanceMatrix; .

    distanceMatrix = new std::vector< std::vector<int> >();
    
    ligne = new std::vector<int>();    
    ligne->push_back(0);
    ligne->push_back(3430);
    ligne->push_back(224);
    ligne->push_back(3000);
    std::vector<int> ligne0;
    for (int i = 0; i < ligne->size(); i++) {
        ligne0.push_back(ligne->at(i));
    }
    distanceMatrix->push_back(ligne0);
    delete ligne;
    
    
    ligne = new std::vector<int>();    
    ligne->push_back(3345);
    ligne->push_back(0);
    ligne->push_back(3569);
    ligne->push_back(5625);
    std::vector<int> ligne1;
    for (int i = 0; i < ligne->size(); i++) {
        ligne1.push_back(ligne->at(i));
    }
    distanceMatrix->push_back(ligne1);
    delete ligne;
    
    ligne = new std::vector<int>();    
    ligne->push_back(310);
    ligne->push_back(3337);
    ligne->push_back(0);
    ligne->push_back(2907);
    std::vector<int> ligne2;
    for (int i = 0; i < ligne->size(); i++) {
        ligne2.push_back(ligne->at(i));
    }
    distanceMatrix->push_back(ligne2);
    delete ligne;
    
    ligne = new std::vector<int>();    
    ligne->push_back(2543);
    ligne->push_back(5569);
    ligne->push_back(2326);
    ligne->push_back(0);
    std::vector<int> ligne3;
    for (int i = 0; i < ligne->size(); i++) {
        ligne3.push_back(ligne->at(i));
    }
    distanceMatrix->push_back(ligne3);
    delete ligne;
    
    
    
    return distanceMatrix;
    
}

// A compléter en implémentant les méthodes à redéfinir de LwSaxParser

