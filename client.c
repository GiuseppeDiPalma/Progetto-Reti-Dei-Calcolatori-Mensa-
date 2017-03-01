//Libreria usata per funzioni e definizioni di librerie
#include "wrapper.h"

struct ordini{
	char ID[24];
	char sede[500];
	char menuscelto[50];
	char orario[3];
	int ritirato;
	};


int main(int argc, char ** argv)
{
	int sockfd,scontrino,pagato,sceltapagamento;
	char recvline[200], root[10]="client";
	char sceltamenu, matricola[11];
	char scontrino_char[24];
	char buff_costo[5];
	
	
	
	struct sockaddr_in servaddr;
	struct ordini ord;
	//Prima famiglia di connessioni per permettere la connessione con un server.
//______________________________________________________________________________
	sockfd = Socket(AF_INET, SOCK_STREAM, 0);
	servaddr.sin_family = AF_INET;
	// - htons memorizza nel campo sin_port della struct sedeaddr
    //          un intero scritto in network order.
	servaddr.sin_port = htons(PORTA);
	// - inet_pton converte la stringa passata come secondo argomento 
    //             in un indirizzo di rete scritto in network order
    //              e lo memorizza nella locazione di memoria puntata dal terzo argomento.
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
	{
		fprintf(stderr,"inet_pton error for %s\n", argv[1]);
		exit (1);
	}
	printf("arg[1]= %s \n servsddr.sin_addr= %d \n servsddr.sin_addr= %d", argv[1], servaddr.sin_addr, &servaddr.sin_addr);
	
//______________________________________________________________________________

	// Connect : usata per connessione al server.
	Connect(sockfd,(struct sockaddr *) &servaddr, sizeof(servaddr));
	//("\t****CLIENT CONNESSO****\n");
	
    /*	Invio il nome "client" per permettere al serverC di riconoscere se sono 
    *	admin o client.
	*/
	write(sockfd, root, sizeof(root));
	/*	Questo Do while permette l'aggiunta della propria matricola e non permette
	*	l'uscita se la matricola inserita supera il size.
	*/
	do{
		printf("\nInserisci la matricola identificativa: ");
		scanf("%s", matricola);
	}while(strlen(matricola)>10);
	
	//scrivo la matricola al server
	write(sockfd, matricola, sizeof(matricola));
	
	// ricevo menu del giorno dal server
	read(sockfd, recvline, sizeof(recvline));
	
	sleep(2);
	
	// ricevo costo del menu del giorno
	
	read(sockfd, buff_costo, sizeof(buff_costo));
	
	printf("\n \nMenu' del giorno:\n\t %s \n", recvline);
	/*	Uso di seguito la funzione atoi per trasformare buff_costo che ricevo dal
	*	server come un'array di caratteri in un int. ( int atoi(const char *str) )
	*/
	int costo = atoi(buff_costo);	

	printf("Il costo del menu è:\n\t %d euro\n ", costo);
	
	while(1)
	{
		do{
			// Scelta associata:
			// 1: si procederà con l'ordine.
			// 2: si uscirà dal client.
			printf("\nVUOI EFFETTUARE L' ORDINE?\n");
			printf("\t[1] - SI\n");
			printf("\t[2] - NO\n");
			scanf("%s", &sceltamenu);
			system("clear");
			
			if(sceltamenu=='1')
			{
				fflush(stdin);
				// Qui avviene la scelta della sede dove preferiamo ritirare il pasto;
				strcpy(ord.menuscelto, recvline);
				printf("Dove vuoi ritirare il pasto?\n");
				printf("(1) - Centro Direzionale\n(2) - Via Acton\n(3) - Sapori Campani\n");
				scanf("%s", &sceltamenu);
			
				// Una volta effettuata la scelta, il nome di questa sede verrà inserito
				// nella struct associata agli ordini.
				system("clear");
				if(sceltamenu=='1') { strcpy(ord.sede,"CentroDirezionale");}
				if(sceltamenu=='2') { strcpy(ord.sede,"ViaActon");}
				if(sceltamenu=='3') { strcpy(ord.sede,"SaporiCampani");}
				
				
				
							
			//Seconda famiglia di connessioni per permettere la connessione di un secondo server.
			//__________________________________________________________________________    
				struct sockaddr_in sedeaddr;
				int sede_fd, portas;
				
				sede_fd = Socket(AF_INET, SOCK_STREAM, 0);
				sedeaddr.sin_family=AF_INET;
				if(strcmp(ord.sede,"CentroDirezionale")==0){portas=3334;}
				if(strcmp(ord.sede,"ViaActon")==0){portas=3335;}
				if(strcmp(ord.sede,"SaporiCampani")==0){portas=3336;}
				// - htons memorizza nel campo sin_port della struct sedeaddr
				//          un intero scritto in network order.
				sedeaddr.sin_port= htons(portas);
				// - inet_pton converte la stringa passata come secondo argomento 
    			//             in un indirizzo di rete scritto in network order
				//              e lo memorizza nella locazione di memoria puntata dal terzo argomento.
				if (inet_pton(AF_INET, argv[1], &sedeaddr.sin_addr) < 0)
					{
						fprintf(stderr,"inet_pton error for %s\n", argv[1]);
						exit (1);
					}
			//__________________________________________________________________________    
				
				
				
				// Ora c'è la scelta dell'ora in cui vogliamo ritirare il pasto.
				do{
				printf("A che ora vuoi ritirare il pasto?\n");
				printf("Inserisci ora: ");
				scanf("%s", &ord.orario);
				}while((atoi(ord.orario))>24);
				
				
				
				// Qui è gestito il pagamento, la consegna della banconota e
				// la consegna del resto.
		
				system("clear");

				// Ricapitolazione dell'ordine su schermo per il cliente.
				// Matricola del richiedente
				// Pasto richiesto
				// Sede scelta
				// Ora prestabilita
				printf("________________________\n");
				printf("|Ricapitolazione ordine\n");
				printf("|Richiedente:   %s     \n", matricola);
				printf("|Pasto:         %s     \n", ord.menuscelto);
				printf("|Sede:          %s     \n", ord.sede);
				printf("|Ora:           %s     \n", ord.orario);
				printf("|_______________________\n");
				sleep(2);
				
				
				printf("\nOrdine inoltrato alla sede scelta;\n\n");
				
				// L'ordine viene poi spedito al server centrale.
				write(sockfd, ord.sede, sizeof(ord.sede));				// invio sede
				write(sockfd, ord.orario, sizeof(ord.orario));			// invio orario

				printf("Vuoi pagare? \n 1 - Si \n 2 - No \n");
				scanf("%d",&sceltapagamento);
				if(sceltapagamento==1)
					{
						pagato=1; 
						printf("Menu Pagato!\n");
					}
				else if(sceltapagamento==2)
					{
						pagato=2; 
						printf("Non Pagato!\n"); 
						exit(0);
					}
				
				
				//  Ricevo l'ID dell'ordine dal serverC
				read(sockfd, &ord.ID, sizeof(ord.ID));
				printf("\nRicevuto id ordine da server c: %s \n", ord.ID);

				// Inizia una connessione con il server sede
				Connect(sede_fd, (struct sockaddr *) &sedeaddr, sizeof(sedeaddr));	//connessione al server
				
				write(sede_fd,&pagato,sizeof(pagato));
				
				close(sockfd);
				int pronto;
				 read(sede_fd, &pronto, sizeof(pronto));
				 
				if(pronto==1)
				{
					printf("Pasto pronto! \nDigita ID SCONTRINO per ritirarlo: \n");

					scanf("%d", &scontrino);
					sprintf(scontrino_char, "%d", scontrino);
					
					write(sede_fd, scontrino_char, sizeof(scontrino_char));
					printf("Spedito scontrino con ID: %s \n", scontrino_char);
					
					read(sede_fd, &ord.ritirato,sizeof(ord.ritirato));
					
					if(ord.ritirato==1)
						{ 
							printf("Ordine Ritirato, arrivederci!\n"); 
						}
					else if(ord.ritirato==2)
						{ 
							printf("ID SBAGLIATO!\n");
						}
					
				 }
				close(sede_fd);
				exit(1);
			}
		}while(sceltamenu=='2');
		
	}
}
