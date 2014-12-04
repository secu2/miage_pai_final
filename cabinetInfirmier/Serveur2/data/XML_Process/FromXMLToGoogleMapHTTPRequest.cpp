#include <iostream>
#include <curl/curl.h>

#include "Adresse.h"
#include "FromXMLToGoogleMapHTTPRequest.h"
#include <string.h>


FromXMLToGoogleMapHTTPRequest::FromXMLToGoogleMapHTTPRequest(std::string id) : LwSaxParser()
{
    this->id = "00" + id;
    id2 =this->id;
    request = "";
    listeAdresses = "";
    state = OTHER;
}

FromXMLToGoogleMapHTTPRequest::~FromXMLToGoogleMapHTTPRequest() {
    
}

std::string FromXMLToGoogleMapHTTPRequest::getGoogleMapHttpRequest() {
    request = ("origins=" + std::string(curl_easy_escape(NULL,listeAdresses.c_str(),0)) + "&destinations=" + std::string(curl_easy_escape(NULL,listeAdresses.c_str(),0)));
    return request;
}

std::string FromXMLToGoogleMapHTTPRequest::findAttribute(const AttributeList& attributeList, std::string attributeName) {
    std::string resultat = "";
    xmlpp::SaxParser::AttributeList::const_iterator iter = attributeList.begin();
    while ((iter != attributeList.end()) && (iter->name != attributeName)) {
        iter++;
    }
    if (iter != attributeList.end()) {
        resultat = iter->value;
    }
    return resultat;
}



void FromXMLToGoogleMapHTTPRequest::on_start_document() {
    
    state=START;
    
}

void FromXMLToGoogleMapHTTPRequest::on_end_document() {
    
    state=OTHER;
    
}


void FromXMLToGoogleMapHTTPRequest::on_start_element(const Glib::ustring& name, const AttributeList& attributes)
{
    
    switch(state) {
	case START: {
            
            
            if (name == "adresse") {
		adresseCourante = new Adresse();
		state=ADRESSE;
            }
            if (name == "visite") {
		state=VISITE;
            }
            
            break;
	}
	case ADRESSE: {
            if (name == "etage") {
                state=ETAGE;
            }
            if (name == "no") {
                state=NUMERO;
            }
            if (name == "rue") {
                state=RUE;
            }
            if (name == "ville") {
                state=VILLE;
            }
            if (name == "codePostal") {
                state=CODEPOSTAL;
            }
            break;
	}
	case PATIENT: {
            if(name == "visite"){
                id2=findAttribute(attributes,"intervenant");
                std::cout << "id: " + id2 << std::endl;
                state = VISITE;
            }
            
            
            break;
        }
	
	default: {
            break;
	}
    }
}

void FromXMLToGoogleMapHTTPRequest::on_end_element(const Glib::ustring& name)
{
    switch(state) {
	case CODEPOSTAL: {
            if (!adresseCabinet){
                listeAdresses= listeAdresses + adresseCourante->getGoogleAdresse();
                //adresseCourante = new Adresse();
                adresseCabinet=true;
                state=START;
            }
            else{
                state = PATIENT;
            }
            
	}
	case RUE: {
            state=ADRESSE;
            break;
	}
	case ETAGE: {
            state=ADRESSE;
            break;
	}
	case VILLE: {
            state=ADRESSE;
            break;
	}
	case NUMERO: {
            state=ADRESSE;
            break;
	}
	case ADRESSE: {
            state=VISITE;
            break;
	}
	case VISITE: {
            if (adresseCabinet && (id == id2)){
                if (adresseCourante->getGoogleAdresse()!=listeAdresses)
                    listeAdresses= listeAdresses + "|" + adresseCourante->getGoogleAdresse();
            }
            state = START;
            break;
	}
	
        
	default: {
            break;
	}
    }
}

void FromXMLToGoogleMapHTTPRequest::on_characters(const Glib::ustring& text)
{
    switch(state) {
        case RUE: {
            adresseCourante->setRue(text);
            std::cout << "setrue: "+ text << std::endl;
            break;
        }
        case CODEPOSTAL: {
            adresseCourante->setCodePostal(text);
            std::cout << "setcp: "+ listeAdresses << std::endl;
            break;
        }
        case VILLE: {
            adresseCourante->setVille(text);
            std::cout << "setville: " + text<< std::endl;
            break;
        }
        case NUMERO: {
            adresseCourante->setNumero(text);
            std::cout << "setnum: " + text << std::endl;
            break;
        }
        default: {
            break;
        }
    }
}


