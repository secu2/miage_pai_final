#include <iostream>

#include <curl/curl.h>
#include "Adresse.h"
#include "FromXMLToGoogleMapHTTPRequest.h"

FromXMLToGoogleMapHTTPRequest::FromXMLToGoogleMapHTTPRequest(std::string id) : LwSaxParser()
{
    this->id = "00" + id;
    
    request = "";
  //  listeAdresses = NULL;
    state = OTHER;
}

FromXMLToGoogleMapHTTPRequest::~FromXMLToGoogleMapHTTPRequest() {
    
}

std::string FromXMLToGoogleMapHTTPRequest::getGoogleMapHttpRequest() {
    request = "origins=" + std::string(curl_easy_escape(NULL,listeAdresses[0].getGoogleAdresse().c_str(),0)) + "&destinations=" + std::string(curl_easy_escape(NULL,listeAdresses[0].getGoogleAdresse().c_str(),0));
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

// A compléter en implémentant les méthodes à redéfinir de LwSaxParser

