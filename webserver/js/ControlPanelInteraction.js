define	( [ 'utils'
		  ]
		, function(utils) {
var ControlPanelInteraction = {
	  createPatient			: function(patientxml){
		 var pat=document.createElement("div");
		 pat.innerHTML = patientxml.querySelector('nom').textContent;
		 pat.draggable = true;
		 pat.dataset.secu = patientxml.querySelector('numéro').textContent;
		 pat.className ="patient";
		 return pat;
		}
		,dragDropIt			: function() {
		var patientTodrag = document.querySelectorAll("*[draggable=true]");
							var dropper = document.querySelectorAll(".tournée");
							var dropperPatientSansAffectation = document.querySelector("#PatientsRestants");
							var saveContent;
							var saveParent;
							var saveChild;
							for(var i=0; i<patientTodrag.length ; i++) {

								patientTodrag[i].addEventListener("dragstart", function(e){

									//dataTransfer.setData('text/plain', 'anything');
									saveContent = e.target.innerHTML;
									saveParent = this.parentNode;
									saveChild = this;

								}, false);
							}
							for(var i=0; i<dropper.length; i++) {
								dropper[i].addEventListener("dragover", function(e){

									e.preventDefault();
									e.stopPropagation();

								}, false);
							}


							dropperPatientSansAffectation.addEventListener("dragover", function(e){

									e.preventDefault();
									e.stopPropagation();

								}, false);

							for(var i=0; i<dropper.length; i++) {
								dropper[i].addEventListener("drop", function(e){
									e.preventDefault();
									e.stopPropagation();
									saveParent.removeChild(saveChild);
									this.appendChild(saveChild);

								}, false);
							}

							dropperPatientSansAffectation.addEventListener("drop", function(e){
									e.preventDefault();
									e.stopPropagation();

									var boutton = document.querySelector("#PatientsRestants button");


									saveParent.removeChild(saveChild);
									this.insertBefore(saveChild, boutton);

								}, false);
								/** Dragend ***/
								for(var i=0; i<dropper.length; i++) {
									dropper[i].addEventListener("dragend", function(e){
										e.preventDefault();
										e.stopPropagation();

										var infirmierID = this.dataset.idInfirmier;

										utils.XHR("POST", "/affectation", {onload : function() {}
																			,variables :{infirmier : infirmierID
																						, patient : e.target.dataset.secu
																						}
																			});

									}, false);
								}
								/** dragend of droperPAtientSansAffect*/
								dropperPatientSansAffectation.addEventListener("dragend", function(e){
									utils.XHR("POST", "/affectation", {onload : function() {}
																			,variables :{infirmier : "none"
																						, patient : e.target.dataset.secu
																						}
																			});

								}, false);

		}
	  ,init					: function() {
		 var self = this;


		 // Remaining patients
		 this.div_remainingPatient		= document.getElementById('PatientsRestants');

		// New patient
		 var div_addPatient = document.getElementById('addPatient');
		 var btAddPatient	= document.getElementById('btAddPatient');
		 var bt_add			= this.div_remainingPatient.querySelector('button');

		 bt_add.onclick=function(){document.getElementById("addPatient").classList.toggle("afficher");}

		 // Emition du formulaire pour l'ajout d'un patient dans la base
		 btAddPatient.onclick = function(){
			 var form = document.querySelector('#formAddPatient');
			 utils.XHR( 'POST', '/addPatient'
					  , { onload: function() {console.log("addPatient répond", this);}
					    , form  : form
					    }
					  );
			 document.getElementById("addPatient").classList.toggle("afficher");
			}

		// Nurses
		 this.tableInfirmiere     = document.getElementById( 'Affectations' );
		 this.bodytableInfirmiere = this.tableInfirmiere.querySelector('tbody');

		 utils.XHR( 'GET', '/data/cabinetInfirmier.xml'
				  , {onload : function() {
								console.log("GET cabinet :", this.responseXML.querySelectorAll('patients'));
								var PatientsDInfirmier = {none:[]};
								var Patients = this.responseXML.querySelectorAll('patients > patient');
								for(var i=0; i<Patients.length;i++)
								{
									var Patient = Patients.item(i);
									var visite = Patient.querySelector('visite');
									if(visite && visite.hasAttribute('intervenant')&&visite.getAttribute("intervenant")!=="")
									{
										var intervenant = visite.getAttribute('intervenant');

										if (typeof PatientsDInfirmier[intervenant] ==="undefined")
										{
											PatientsDInfirmier[intervenant] =[];
										}
										PatientsDInfirmier[intervenant].push(Patient);
									}
									else
										{
											PatientsDInfirmier.none.push(Patient);
										}

								}

					// Création de la représentation des infirmiers et de leur tournée
				    var infirmiers = this.responseXML.querySelectorAll('infirmiers > infirmier');
					for(var i=0;i<infirmiers.length;i++) {
						var infirmier = infirmiers.item(i);
						//Créer 1 ligne dans le tableau HTML pour cet infirmier
						 var ligneHTML = document.createElement("tr");
						 ligneHTML.innerHTML = '<td class="infirmier"></td><td class="tournée"></td>';
						 ligneHTML.querySelector('td.infirmier').innerHTML = infirmier.querySelector('nom').textContent;

						 var tdTournée=ligneHTML.querySelector('td.tournée');
								tdTournée.dataset.idInfirmier = infirmier.getAttribute("id");
							// tdTournée doit être définit comme zone de drop

						 var patients = PatientsDInfirmier[infirmier.getAttribute("id")] || [];
						 for(var p=0;p<patients.length;p++) {
							tdTournée.appendChild(ControlPanelInteraction.createPatient(patients[p]));
							}
						 document.querySelector("#Affectations > tbody").appendChild(ligneHTML);
						}
					// Création des patients qui n'ont pas été affectés
					 var divPatientsNonAffectés = document.getElementById('PatientsRestants');
					 divPatientsNonAffectés.ondragover = function(e) {if(ControlPanelInteraction.draggedElement) {e.preventDefault();}}
					 divPatientsNonAffectés.ondrop = function(e) {

														 // Détacher l'élément draggedElement
														 // L'attacher à la zone de drop
														 // Faire un POST sur le serveur

														}
					 var patients = PatientsDInfirmier.none;
					 for(var p=0;p<patients.length;p++) {
						 divPatientsNonAffectés.appendChild( ControlPanelInteraction.createPatient(patients[p]) );
						}
						ControlPanelInteraction.dragDropIt();
					}
				}
			);

}

}
return ControlPanelInteraction;
}
);
