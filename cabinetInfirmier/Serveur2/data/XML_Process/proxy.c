#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "libcomm/communicate.h"

#define SOCKET_ERROR -1
#define INVALID_SOCKET -1

#define TAILLE_BUFFER 4096

#define PORT_NODEJS 8046

void mort_fils() {
	int status;
	wait(&status);
}

int main(int argc, char * argv[]) {
	//Gestion de signal
	struct sigaction sig;
	sig.sa_handler=mort_fils;
	sig.sa_flags=SA_RESTART;
	sigaction(SIGCHLD, &sig, NULL);

	int port = 80;
	int indice;

	int pid_fils, pid_execution_app_ext;

	int sock, csock, sock_nodejs, sock_cacheujf;

	struct sockaddr_in sin = { 0 };
	struct sockaddr_in csin = { 0 };
	struct sockaddr_in sin_nodejs = { 0 };
	struct sockaddr_in sin_cacheujf = { 0 };

	socklen_t sinsize = sizeof(csin);

	struct hostent *hostinfo_nodejs = NULL;
	const char *hostname_nodejs = "127.0.0.1";

	struct hostent *hostinfo_cacheujf = NULL;
	const char *hostname_cacheujf = "www-cache.ujf-grenoble.fr";

	char buffer_in[TAILLE_BUFFER];
	char buffer_out[TAILLE_BUFFER];

	char chaine_intercept_inf[]="/INFIRMIERE";
	char chaine_intercept_gest[] = "interface-infirmiere";

	int size_in;//, size_out;

	int file_output_app_ext;
	char *s_id_post_inf;
	int status_pid_app_ext;
	int id_post_inf;

	/**
	 * Récupération des paramètres, numéro de port ...
	 */

	if (argc>1) {
		for(indice=1;indice<argc;indice=indice+2) {
			if (argv[indice][0]=='-'&&argv[indice][2]=='\0') {
				switch (argv[indice][1]) {
					case 'p' :
						port=atoi(argv[indice+1])%65535;
					break;
					
					default:
					break;	
				}
			}
		}
	}
	
	/*
	 * Création des structures, sockets, ...
	 */

	//Créatin de la socket d'écoute

	sock = socket(AF_INET, SOCK_STREAM, 0);

	if(sock == INVALID_SOCKET) {
	    perror("Erreur avec la procedure socket()");
	    exit(errno);
	}


	//Attachement de la socket

	sin.sin_addr.s_addr = htonl(INADDR_ANY); /* nous sommes un serveur, nous acceptons n'importe quelle adresse */

	sin.sin_family = AF_INET;

	sin.sin_port = htons(port);

	if(bind (sock, (struct sockaddr *) &sin, sizeof(sin)) == SOCKET_ERROR) {
	    perror("Erreur avec la procedure bind()");
	    exit(errno);
	}

	//Ouverture du service
	if(listen(sock, 5) == SOCKET_ERROR) { // Limité a 5 connexions simultanées
	    perror("Erreur avec la procedure listen()");
	    exit(errno);
	}

	while(1) {
		/*
		 * En attente de connexion
		 */

		if((csock = accept(sock, (struct sockaddr *) &csin, &sinsize)) == INVALID_SOCKET) {
		    perror("Erreur avec la procedure accept()");
		    exit(errno);
		}

		//Création d'un processus fils

		pid_fils=fork();

		switch(pid_fils) {
			case 0 :
				close(sock);

				/*
				 * Creation des sockets pour la connexion a l'exterieur cache ujf et socket pour nodejs
				 */

				// Connexion au cache UJF

				sock_cacheujf = socket(AF_INET, SOCK_STREAM, 0);

				if(sock_cacheujf == INVALID_SOCKET) {
				    perror("Erreur avec la procedure socket()");
				    exit(errno);
				}

				hostinfo_cacheujf = gethostbyname(hostname_cacheujf);

				if (hostinfo_cacheujf == NULL) {
				    fprintf (stderr, "Unknown host %s.\n", hostname_cacheujf);
				    exit(EXIT_FAILURE);
				}

				sin_cacheujf.sin_addr = *(struct in_addr *) hostinfo_cacheujf->h_addr;
				sin_cacheujf.sin_port = htons(3128);
				sin_cacheujf.sin_family = AF_INET;

				if(connect(sock_cacheujf,(struct sockaddr *) &sin_cacheujf, sizeof(struct sockaddr)) == SOCKET_ERROR)
				{
				    perror("Erreur avec la procedure connect() cacheujf");
				    exit(errno);
				}

				// Connexion au serveur nodejs

				sock_nodejs = socket(AF_INET, SOCK_STREAM, 0);

				if(sock_nodejs == INVALID_SOCKET) {
				    perror("Erreur avec la procedure socket()");
				    exit(errno);
				}

				hostinfo_nodejs = gethostbyname(hostname_nodejs);

				if (hostinfo_nodejs == NULL) {
				    fprintf (stderr, "Unknown host %s.\n", hostname_nodejs);
				    exit(EXIT_FAILURE);
				}

				sin_nodejs.sin_addr = *(struct in_addr *) hostinfo_nodejs->h_addr;
				sin_nodejs.sin_port = htons(PORT_NODEJS);
				sin_nodejs.sin_family = AF_INET;

				if(connect(sock_nodejs,(struct sockaddr *) &sin_nodejs, sizeof(struct sockaddr)) == SOCKET_ERROR)
				{
				    perror("Erreur avec la procedure connect() nodejs");
				    exit(errno);
				}

				/*
				 * Le code suivant effectue presque la meme chose que communicate mais pas encore tout a fait ;)
				 */


				//Reception de donnée depuis client
				//traitement,
					//Envoi de donnée au serveur nodejs
					//ou
					//Envoi de données a gmap
				//reception depuis ce dernier
				//envoi au client ou traitement puis envoi au client
			
/*
				do {
					memset(buffer_out, 0, TAILLE_BUFFER);
					size_in=recv(csock, buffer_in, TAILLE_BUFFER,0);
					if(size_in == 0) {
						printf("Client in disconnected");
						fflush(stdout);
					}
					else if(size_in == -1) {
						perror("Erreur avec la procedure recv() in ");
						exit(errno);
					}

printf("Buffer in : \n %s\n", buffer_in);
					if(send(sock_nodejs, buffer_in, size_in, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}

					printf("\nsize in : %d\n", size_in);


				} while (size_in>0&&size_in<TAILLE_BUFFER);
			
				memset(buffer_out, 0, TAILLE_BUFFER);
				while((size_out=recv(sock_nodejs, buffer_out, TAILLE_BUFFER-1,0))>0) {
				//if((size_out=recv(sock_nodejs, buffer_out, TAILLE_BUFFER-1,0))>0) {
printf("Buffer out : \n %s\n", buffer_out);
					if(send(csock, buffer_out, size_out, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}
				}	
	
				if(size_out == 0) {
					printf("Client out disconnected");
					fflush(stdout);
				}
				else if(size_out == -1) {
					perror("Erreur avec la procedure recv() out ");
					exit(errno);
				}		
*/

				/*
				 * A la place on utilise ca, on recupere le premier paquet on analyse et en fonction de ca on choisi
				 */

				memset(buffer_out, 0, TAILLE_BUFFER);

				size_in=recv(csock, buffer_in, TAILLE_BUFFER,0);

				if(size_in == 0) {
					printf("Client in disconnected");
					fflush(stdout);
				}
				else if(size_in == -1) {
					perror("Erreur avec la procedure recv() in ");
					exit(errno);
				}		

				if(strstr(buffer_in, chaine_intercept_inf)!=NULL) {
				/*
					if(send(sock_cacheujf, buffer_in, size_in, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}

					communicate(csock, sock_cacheujf, buffer_in);
				*/
					/*
					 * On recoit une requete /INFIRMIERE on va extraire l'id et lancer l'application
					 */

					printf("\nBuffer recu : \n %s \n \n", buffer_in);
					
					if((s_id_post_inf=strstr(buffer_in,"id="))==NULL) {
						printf("\n Attention ! Pas d'id trouvé \n");
					}
					else {
						switch(pid_execution_app_ext=fork()) {
							case 0 :

						file_output_app_ext = open("tmp/tmp_output_app_ext.tmp", O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP);

								printf("\n id : %d \n",atoi(s_id_post_inf+3));

								dup2(file_output_app_ext, STDOUT_FILENO);

								//close(file_output_app_ext);

								id_post_inf=atoi(s_id_post_inf+3);
id_post_inf=id_post_inf;
//system("../infirmiere/Serveur/XML_Process/testParsers 1 1 > tmp/tmp_output_app_ext.tmp");
								execl("testParsers","testParsers", "1", "1",(char *)0);
								exit(EXIT_FAILURE); // En cas d'echec d'execl
							break;

							default :

								printf("en attente mort execution app cpp\n");

								waitpid(pid_execution_app_ext, &status_pid_app_ext, 0);

								printf("le fils est mort, vive le fils !");

								file_output_app_ext = open("tmp/tmp_output_app_ext.tmp", O_RDWR);

								if(read(file_output_app_ext, buffer_out, TAILLE_BUFFER)==-1) {
									perror("error read");
									exit(EXIT_FAILURE);
								}

								printf("\n lu dans fichier : %s ", buffer_out);

								close(file_output_app_ext);
							break;
						}
					}

					/*
					 * C'est pour google maps on fait le traitement nécessaire
					 */
				}
				else if(strstr(buffer_in, chaine_intercept_gest)!=NULL) {
					/*
					 * Requete relative a l'interface de gestion
					 */
					if(send(sock_nodejs, buffer_in, size_in, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}

					communicate(csock, sock_nodejs, buffer_in);
				}
				else {
					/*
					 * La chaine n'a pas été trouvée on envoie toutes les requetes sur nodejs, communicate fait ca apeu près bien.
					 */
					if(send(sock_cacheujf, buffer_in, size_in, 0) < 0) {
						perror("Erreur avec la procedure send()");
						exit(errno);
					}

					communicate(csock, sock_cacheujf, buffer_in);
				}

				close(csock);
				close(sock_nodejs);

				return 0;
			break;

			default :
				close(csock);
			break;
		}
	}

	close(sock);

	return 0;
}
