<?xml version="1.0" encoding="UTF-8"?>

<!--
    Document   : infosInfirmiers.xsl
    Created on : 15 octobre 2014, 10:06
    Author     : clementbrocard
    Description:
        Purpose of transformation follows.
-->

<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
                xmlns:cab="http://localhost/schema/cabinetInfirmier"
                xmlns:act="http://www.ujf-grenoble.fr/l3miage/actes"
                xmlns="http://www.w3.org/TR/html4/"
                version="1.0">
    
    <xsl:output method="html"/>
    
    <!-- L'id de l'infirmière -->
    <xsl:param name="destinedId" select="001"/>
    <!-- Liste des actes -->
    <xsl:variable name="actes" select="document('actes.xml', /)/act:ngap"/>

    <xsl:template match="/">       
        <xsl:variable name="visitesDuJour" select="//cab:patient/cab:visite[@intervenant=$destinedId]" />
        <xsl:variable name="nbPatients" select="count($visitesDuJour)" />
        <html>
            <head>
                <title>Bonjour <xsl:value-of select="//cab:infirmier[@id=$destinedId]/cab:prenom"/></title>
            </head>
            <body>
                <h2>Bonjour <xsl:value-of select="//cab:infirmier[@id=$destinedId]/cab:prenom"/>,</h2>
                <xsl:choose>
                    <xsl:when test="$nbPatients &lt; 1">
                        <p>vous n'avez aucun patient pour le moment</p>
                    </xsl:when>
                    <xsl:otherwise>
                        <p>aujourd'hui vous avez <xsl:value-of select="$nbPatients" /> patient<xsl:if test="$nbPatients &gt; 1">s</xsl:if></p>
                    </xsl:otherwise>
                </xsl:choose>
                <xsl:apply-templates select="$visitesDuJour/.."/>
            </body>
        </html>
    </xsl:template>
    
    <xsl:template match="cab:patient">
        <table border="1">
            <tr>
                <th>Nom</th>
                <td>
                    <xsl:value-of select="cab:nom" />
                </td>
            </tr>
            <tr>
                <th>Prénom</th>
                <td>
                    <xsl:value-of select="cab:prenom" />
                </td>
            </tr>
            <tr>
                <th>Adresse</th>
                <td>
                    <xsl:apply-templates select="cab:adresse"/>
                </td>
            </tr>
            <xsl:apply-templates select="cab:visite"/>
        </table>
    </xsl:template>
    
    <xsl:template  match="cab:adresse">
        <table>
            
            <tr>
                <td>
                    <xsl:if test="cab:no">
                        <xsl:value-of select="cab:no"/>
                        <xsl:text> </xsl:text>
                    </xsl:if> 
                    <xsl:value-of select="cab:rue"/>
                </td>
            </tr>
            <tr>
                <td>
                    <xsl:value-of select="cab:codePostal"/>
                    <xsl:text> </xsl:text>
                    <xsl:value-of select="cab:ville"/>
                </td>
            </tr>
            <xsl:if test="cab:etage">
                <tr>    
                    <td>
                        Étage<xsl:text> </xsl:text>
                        <xsl:value-of select="cab:etage"/>
                    </td>
                </tr>
            </xsl:if>
        </table>
    </xsl:template>
    
    <xsl:template match="cab:visite">
        <xsl:variable name="idActe" select="cab:acte/@id" />
        <tr>
            <xsl:element name="th">
                <xsl:attribute name="rowspan">
                    <xsl:value-of select="count(cab:acte)"/>
                </xsl:attribute>
                Soins:
            </xsl:element>
            <xsl:if test="count(cab:acte) &lt; 1">
                <td>Aucun</td>
            </xsl:if>
            <xsl:if test="count(cab:acte) &gt; 0">
                <td><xsl:value-of select="$actes//act:acte[@id=$idActe]" /> (<xsl:value-of select="$idActe"/>)</td>
            </xsl:if> 
        </tr>
        <xsl:apply-templates select="cab:acte[position()>1]"/>
    
        
    
    </xsl:template>
    
    <xsl:template match="cab:acte">x
        <xsl:variable name="idActe" select="@id" />
        <tr>
            <td><xsl:value-of select="$actes//act:acte[@id=$idActe]" /> (<xsl:value-of select="$idActe"/>)</td>
        </tr>
    </xsl:template>

</xsl:stylesheet>
