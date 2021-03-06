#include <iostream>
#include <stdlib.h> 
#include <stdio.h>
#include "FromGoogleMapXMLToDistanceTable.h"

// Constructeur
FromGoogleMapXMLToDistanceTable::FromGoogleMapXMLToDistanceTable() : LwSaxParser() {
    state = UNKNOWN;
    adresseCourante = "";
    adresses = new std::vector<std::string>();
    ligne = NULL;
    distanceMatrix = new std::vector< std::vector<int> >();
    nbligne=0;
}


FromGoogleMapXMLToDistanceTable::~FromGoogleMapXMLToDistanceTable() {
    
}


std::vector<std::string> * FromGoogleMapXMLToDistanceTable::getAdresses() {
    // Ici, on triche pour que le test fonctionne du premier coup
    // Ces lignes sont à remplacer par un vrai parcours du document XML
    // Cette méthode ne devra plus contenir que return adresses; .
    
    /*
     adresses->push_back("46 Avenue Félix Viallet, IUT 2, 38031 Grenoble, France");
     adresses->push_back("Rond-point de la Croix de Vie, 38700 La Tronche, France");
     adresses->push_back("Rue Casimir Brenier, 38000 Grenoble, France");
     adresses->push_back("25 Rue des Martyrs, 38000 Grenoble, Francev");
     */  
    return adresses;
}

std::vector< std::vector<int> > * FromGoogleMapXMLToDistanceTable::getDistances() {
    
    
    //afficherMatrice();
    return distanceMatrix;
    
}

void FromGoogleMapXMLToDistanceTable::symetrisation(){
    std::cout << "sym" << std::endl;
    for (unsigned i=0; i<distanceMatrix->size(); i++){
        for (unsigned j=i; j<distanceMatrix->size(); j++){
            if(distanceMatrix->at(i).at(j) != distanceMatrix->at(j).at(i) ){
                distanceMatrix->at(i).at(j) = (distanceMatrix->at(i).at(j) + distanceMatrix->at(j).at(i))/2;
                distanceMatrix->at(j).at(i) = distanceMatrix->at(i).at(j);
            }
        }
    } 
}

void FromGoogleMapXMLToDistanceTable::on_start_document() {
    state = START;
    adresses = new std::vector<std::string>();
    distanceMatrix = new std::vector< std::vector<int> >() ;
}

void FromGoogleMapXMLToDistanceTable::on_end_document() {
    symetrisation();
}

void FromGoogleMapXMLToDistanceTable::on_start_element(const Glib::ustring& name, const AttributeList& attributes)
{
    switch(state){
	case START: {
            if(name == "origin_address")
		state=ORIGIN_ADDRESS;
            if(name == "row") {
		ligne = new std::vector<int>();    
		state=ROW;
		std::cout << "row" << std::endl;
            }
            break;
	}
	case ORIGIN_ADDRESS: {
            
            break;
	}
	case ROW: {
            
            if(name == "element")
		state=ELEMENT;
            break;
	}
	case ELEMENT: {
            
            if(name == "distance")
                state=DISTANCE;
            
            break;
	}
	case DISTANCE: {
            if(name == "value")
		state=VALUE;
            break;
	}
	default: {
            
            break;
	}
    }
    
}

void FromGoogleMapXMLToDistanceTable::on_end_element(const Glib::ustring& name)
{
    /*switch(state){
     case ORIGIN_ADDRESS: {
     state=START;
     std::cout << "start" << std::endl;
     break;
     }
     
     case ROW: {
     distanceMatrix->push_back(*ligne);
     state=START;
     break;
     }
     
     case ELEMENT: {
     if (name == "element")
     state=ROW;
     break;
     }
     case VALUE: {
     if(name == "value")
     state=DISTANCE;
     break;
     }
     case DISTANCE: {
     if(name == "distance")
     state=ELEMENT;
     break;
     }
     default: {
     
     break;
     }*/
    if(name == "row"){
        distanceMatrix->push_back(*ligne);
        state = START;
    }
    else if(name == "origin_address"){
        adresses->push_back(adresseCourante);
        
        std::cout << adresseCourante << std::endl;
        adresseCourante = "";
        state = START;
    }
    else if(name == "destination_address"){
        state = START;
    }
    else if(name == "element"){
        state = ROW;
    }
    else if(name == "duration"){
        state = ELEMENT;
    }
    else if(name == "distance"){
        state = ELEMENT;
    }
    else if(name == "value" and state == VALUE){
        state = DISTANCE;
    }
}

void FromGoogleMapXMLToDistanceTable::on_characters(const Glib::ustring& text)
{
    switch(state){
        case ORIGIN_ADDRESS: {
            adresseCourante += text;
            break;
        }
        case VALUE: {
            //std::cout << "valeur ajoutée : " + text << std::endl;
            ligne->push_back(atoi(text.c_str()));			
            break;
        }
        case ROW: {
            
            break;
        }
        default : {
            break;
        }
    }
}


void FromGoogleMapXMLToDistanceTable::afficherAdresses() {
    
    for (unsigned i=0; i<adresses->size(); i++)
        std::cout << adresses->at(i) << std::endl;
}

void FromGoogleMapXMLToDistanceTable::afficherMatrice() {
    
    for (unsigned i=0; i<distanceMatrix->size(); i++){
        for (unsigned j=0; j<distanceMatrix->size(); j++){
            std::cout << " " + distanceMatrix->at(i).at(j) << std::endl;
	}
    } 
}

