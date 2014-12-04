var AMIVAL = 3.15;
var AISVAL = 2.65;
var DIVAL = 10.0;
var totalFacture = 0.0;
NodeList.prototype.forEach = Array.prototype.forEach;
function afficherFacture(prenom, nom, actes)
{
    totalFacture = 0.0;
    var text = "<html>\n";
    text +=
            " <head>\n\
<title>Facture</title>\n\
<link rel='stylesheet' type='text/css' href='css/mystyle.css'/>\n\
</head>\n\
<body>\n";
    text += "Facture pour " + prenom + " " + nom + "<br/>";
    // Trouver l'adresse du patient
    var xmlDoc = loadXMLDoc("../cabinetInfirmier.xml");
    var patients = xmlDoc.getElementsByTagName("patient");
    var i = 0;
    var found = false;
    while ((i < patients.length) && (!found)) {
        var patient = patients[i];
        var localNom = patient.getElementsByTagName("nom")[0].childNodes[0].nodeValue;
        var localPrenom = patient.getElementsByTagName("prenom")[0].childNodes[0].nodeValue;
        if ((nom === localNom) && (prenom === localPrenom)) {
            found = true;
        }
        else {
            i++;
        }
    }
    if (found) {
        text += "Adresse: ";
        // On rÃ©cupÃ¨re l'adresse du patient
        var adresse = patient.getElementsByTagName("adresse");
        // adresse = ... Ã complÃ©ter par une expression DOM
        text += adresseToText(adresse);
        text += "<br/>";
        var nSS = patient.getElementsByTagName("numero")[0].childNodes[0].nodeValue;
        console.log(nSS);
        // nss = rÃ©cupÃ©rer le numÃ©ro de sÃ©curitÃ© sociale grÃ¢ce Ã une expression DOM
        text += "Numéro de sécurité sociale: " + nSS + "\n";
    }
    text += "<br/>";
    // Tableau rÃ©capitulatif des Actes et de leur tarif
    text += "<table border='1' bgcolor='#CCCCCC'>";
    text += "<tr>";
    text += "<td> Type </td> <td> Clé </td> <td> Intitulé </td> <td> Coef </td> <td> Tarif </td>";
    text += "</tr>";
    var acteIds = actes.split(" ");
    for (var j = 0; j < acteIds.length-1; j++) {
        text += "<tr>";
        var acteId = acteIds[j];
        text += acteTable(acteId);
        text += "</tr>";
    }
    text += "<tr><td colspan='4'>Total</td><td>" + totalFacture + "€</td></tr>\n";
    text +="</table>";
    text +=
            " </body>\n\
</html>\n";
    return text;
}
// Mise en forme d'un noeud adresse pour affichage en html
function adresseToText(adresse)
{
    var str = "";
    var adresseClassic = [3];
    var extra = "";
    var children = [].slice.call(adresse[0].children);
    children.forEach(function(element){
        if(element.nodeName == "no"){
            adresseClassic[0] = element.childNodes[0].nodeValue;
        }else if(element.nodeName == "rue"){
            adresseClassic[1] = element.childNodes[0].nodeValue;
        }else if(element.nodeName == "codePostal"){
            adresseClassic[2] = element.childNodes[0].nodeValue;
        }else if(element.nodeName == "ville"){
            adresseClassic[3] = element.childNodes[0].nodeValue;
        }else{
            extra += element.nodeName + ": " + element.childNodes[0].nodeValue;
        }
    });
    adresseClassic.forEach(function(element){
        str += element + " ";
    });
    str += "<br />";
    // Mise en forme de l'adresse du patient
    // A complÃ©ter
    return str + " " + extra;
}
function acteTable(acteId)
{
    var str = "";
    var xmlDoc = loadXMLDoc("../actes.xml");
    var actes = xmlDoc.getElementsByTagName("acte");
    // actes = rÃ©cupÃ©rer les actes de xmlDoc
    // ClÃ© de l'acte (3 lettres)
    var cle;
    // Coef de l'acte (nombre)
    var coef;
    // Type id pour pouvoir rÃ©cupÃ©rer la chaÃ®ne de caractÃ¨res du type
    // dans les sous-Ã©lÃ©ments de types
    var typeId;
    // ChaÃ®ne de caractÃ¨re du type
    var type = "";
    // ...
    // IntitulÃ© de l'acte
    var intitule;
    // Tarif = (lettre-clÃ©)xcoefficient (utiliser les constantes
    var AMIVAL = 3.15;
    var AISVAL = 2.65;
    var DIVAL = 10.0;
    // (cf http://www.infirmiers.com/votre-carriere/ide-liberale/la-cotation-des-actes-ou-comment-utiliser-la-nomenclature.html)
    var tarif = 0.0;
    // Trouver l'acte qui correspond
    var i = 0;
    var found = false;
    // A dÃ©-commenter dÃ¨s que actes aura le bon type...
    var i = 0;
    while ((i < actes.length) && (!found)) {
        if(actes[i].id === acteId){
            found = true;
        }
        if (found) {
            // A complÃ©ter
            cle = actes[i].attributes["clé"].nodeValue;
            coef = actes[i].attributes["coef"].nodeValue;
            type = actes[i].attributes["type"].nodeValue;
            if(cle === "AMI"){
                typeId = "AMIVAL";
                tarif = AMIVAL;
            }if(cle === "AIS"){
                typeId = "AISVAL";
                tarif = AISVAL;
            }if(cle === "DIV"){
                typeId = "DIVAL";
                tarif = DIVAL;
            }
            tarif = tarif*coef;
            intitule = actes[i].firstChild.nodeValue.trim();
        }
        i++;
    }
    str += "<td>" + type + "</td>";
    str += "<td>" + cle + "</td>";
    str += "<td>" + intitule + "</td>";
    str += "<td>" + coef + "</td>";
    str += "<td>" + tarif + "€</td>";
    console.log(str);
    // A modifier
    totalFacture += tarif;
    return str;
}
// Fonction qui charge un document XML
function loadXMLDoc(docName)
{
    console.debug("Loading "+docName);
    var xmlhttp;
    if (window.XMLHttpRequest)
    {// code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    }
    else
    {// code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.open("GET", docName, false);
    xmlhttp.send();
    xmlDoc = xmlhttp.responseXML;
    return xmlDoc;
}
