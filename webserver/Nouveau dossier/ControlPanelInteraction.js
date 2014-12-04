define	( [ 'utils'
		  ]
		, function(utils) {
var ControlPanelInteraction = {
	  init					: function() {
		 var self = this;
		 
		 // Remaining patients
		 this.div_remainingPatient		= document.getElementById('PatientsRestants');
		 
		// New patient
		 var div_addPatient = document.getElementById('addPatient');
		 var btAddPatient	= document.getElementById('btAddPatient');
		 var bt_add			= this.div_remainingPatient.querySelector('button');

		// Nurses
		 this.tableInfirmiere     = document.getElementById( 'Affectations' );
		 this.bodytableInfirmiere = this.tableInfirmiere.querySelector('tbody');

		
/*
	function createPatientHTML() {
		var infirmiers = doc.querySelectorAll("infirmiers>infirmier");

		for (var i=0 ; i<infirmier.length,i++) {
		var infirmier =infirmier.item(i);

		infirmierHTML.innerHTML ="<td class"infirmier"></td><td class="tournee"><td>";

		var tournee = infirmier.querySelector("td tournee");
		var patients =PatientsDeinfirmiers[infirmier.getattribute("id")]

						for (var p = 0; p<patients.length; p++) {
							tourneHTML.appendChild(ControlPanelInteraction.createPatient(patients[p]));
							}
					document.querySelector("#table>tbody").appendChild(infirmierHTML);

		}
	}

*/


function createPatient(pat,pn)
{

				var StructHTML = document.createElement("div");	
				var nomp = pat.querySelector('nom').textContent;
				var prenomp = pat.querySelector('prénom').textContent;	
				StructHTML.innerHTML ='</br><span id='+pn+'>'+nomp+' '+ prenomp +'</span>';		

	return StructHTML;

}


		 utils.XHR( 'GET', '/data/cabinetInfirmier.xml'
				  , {onload : function() {
							 var doc = this.responseXML;
							 var patients = doc.querySelectorAll('patients>patient');
							 var patientsIntervenant ={none:[]};
								 
							 for(var i=0; i<patients.length;i++){
								var patient=patients.item(i);
								var visite=patient.querySelector('visite');
								  if(visite && visite.hasAttribute('intervenant') && visite.getAttribute('intervenant')!=''){
								    var intervenant=visite.getAttribute('intervenant');
									if(typeof patientsIntervenant[intervenant] ==="undefined")
										{
										 patientsIntervenant[intervenant]=new Array();
										}
									 patientsIntervenant[intervenant].push(patient);
									}
									else 
									{	
									 patientsIntervenant.none.push(patient);
									}
								 
								 }
							console.log( patientsIntervenant[intervenant]);
							/*
							for (var i = 0; i<patients.length; i++) {
								uItem = document.createElement('p');
								uItemText = document.createTextNode(patients.item(i).nom);
								console.log(patients.item(i).nom);
								uItem.appendChild(uItemText);
								}
							*/

					var infirmiers = doc.querySelectorAll("infirmiers>infirmier");

					for (var i=0 ; i<infirmiers.length;i++) {
					var infirmier =infirmiers.item(i);

					var infirmierHTML = document.createElement("tr");
					var nomi = infirmier.querySelector('nom').textContent;
					var prenomi = infirmier.querySelector('prénom').textContent;
					infirmierHTML.innerHTML ='<td class="infirmier">'+nomi+' '+ prenomi+'</td><td class="tournee"></td>';

					var tourneHTML = infirmierHTML.querySelector("td.tournee");
					var patients =patientsIntervenant[infirmier.getAttribute("id")]||[]

									for (var p = 0; p<patients.length; p++) {
										tourneHTML.appendChild(createPatient(patients[p],"pn"));
										}
					document.querySelector("#Affectations>tbody").appendChild(infirmierHTML);

									}


					var patientN=patientsIntervenant.none;
					var pr=document.querySelector("#PatientsRestants");
						for (var p = 0; p<patientN.length; p++) {
							pr.appendChild(createPatient(patientN[p],"pr"));
							}

						}
								}
				    
				  );
		 }
};





return ControlPanelInteraction;
});
