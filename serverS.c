//SERVER SEDE 

#include "wrapper.h"

int main(int argc, char **argv)
{
	int connfd, listenfd, n;
    int client_connfd, client_listenfd;
    int pagato;
    char utente_menu[50], sede[500] /*, ID[11]*/;
    char matricola[11];
	char sedescelta[500];
	int orario_int;

	struct sockaddr_in servaddr;
	struct sockaddr_in clientaddr;
	
	char orario[3];
	
	pid_t pid;

// Famiglia di connessione per permettere connessine tra Server Sede e Server Centrale
//____________________________________________________________________
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);
	// - famiglia AF_INET IPv4 Internet Protocols
	// - tipo SOCK_STREAM 
	// - 0 protocollo
    servaddr.sin_family = AF_INET; /* address type is INET */

    int porta;
    if(strcmp(argv[2],"centrodirezionale")==0) { porta=3333; }
    if(strcmp(argv[2],"viaacton")==0) { porta=4444; }
    if(strcmp(argv[2],"saporicampani")==0) { porta=5555; }
    
    // - htons      memorizza nel campo sin_port della struct sedeaddr
    //              un intero scritto in network order.
    // - sin_port   Porta in network order
    servaddr.sin_port = htons(porta);

    printf("porta: %d \n", porta);
    printf("network order htons(porta): %d \n", servaddr.sin_port);
    
    // - L' applicazione accettera connessioni
    //   da qualsiasi indirizzo associato al server.
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    Listen(listenfd, 32);
    /*
    La funzione listen è definita sia in ambiente windows che in ambiente linux 
    ed il suo prototipo è il seguente

    int listen(int sockfd, int backlog)

    dove con sockfd 
        si indica la socket che si pone in ascolto, 
    e con backlog 
        si indica il numero massimo di connessioni in attesa di completamento 
        contemporaneamente gestibili dalla socket. Queste connessioni sono quelle 
        connessioni pendenti, connessioni richieste dai client ma non ancora accettate 
        dal server tramite la funzione accept. 
    */
    
    
//______________________________________________________________________________

    
// Famiglia di connessione per permettere connessine tra Server Sede e Clientff
//______________________________________________________________________________

    client_listenfd = Socket(AF_INET, SOCK_STREAM, 0); //socket per conn del client
    // - famiglia AF_INET IPv4 Internet Protocols
	// - tipo SOCK_STREAM 
	// - 0 protocollo
	
    memset((void*) &clientaddr,0, sizeof(clientaddr));  /* clear server address */
    clientaddr.sin_family = AF_INET; /* address type is INET */
    
    // - L' applicazione accettera connessioni
    //   da qualsiasi indirizzo associato al server.
    clientaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int portac;
    if(strcmp(argv[2],"centrodirezionale")==0) { portac=3334; }
    if(strcmp(argv[2],"viaacton")==0) { portac=3335; }
    if(strcmp(argv[2],"saporicampani")==0) { portac=3336; }
    
    // - htons      memorizza nel campo sin_port della struct sedeaddr
    //              un intero scritto in network order.
    // - sin_port   Porta in network order
    clientaddr.sin_port = htons(portac);

    Bind(client_listenfd, (struct sockaddr *) &clientaddr, sizeof(clientaddr));
    Listen(client_listenfd, 32); //mi metto in ascolto
//______________________________________________________________________________

    if(strcmp(argv[2],"centrodirezionale")==0)
        {
            while(1)
            	{	    
            	    printf("\n\t****SERVER GESTIONE SEDI CENTRO DIREZIONALE****\n");
            	    //  ACCEPT : accetta la connessione con il Server Centrale
            	    connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);
            	    printf("connfd= %d", connfd);
            	    
                    if((pid=fork())<0)//Effettuo Fork
                        { 
                            perror("errore nella fork()");
                            exit(-1);
                        }
                    if(pid==0)//figlio
                    {
                        close(listenfd);
                        //read(connfd, sede, sizeof(sede));//Ricevo il nome della sede scelto dal Client
                        strcpy(sedescelta, "CentroDirezionale");
                        //sedescelta="CentroDirezionale";
                        
                        
                        //Ricevo il resto dell' ordine
                        read(connfd, orario, sizeof(orario));
                        read(connfd, matricola, sizeof(matricola));                
                        read(connfd, utente_menu, sizeof(utente_menu));   
                        
                        orario_int=atoi(orario);
                        
                        printf("- Matricola Utente: %s\n- Menu Scelto: %s \n- Ora: %s \n", matricola, utente_menu, orario);
                        printf("Genero ID Ordine....\n");
                            
                        //Genero ID///////////////////////////////
            		    int prefisso_int=100;
            		    char ID[24];
            		    sprintf(ID, "%d", prefisso_int);
            		    srand(time(NULL));
            		    int codice_int;    
            		    codice_int=rand() % 100 + 1;
            		    char codice_char[4];
            		    sprintf(codice_char, "%d", codice_int);
            		    strcat(ID,codice_char);
            		    ///////////////////////////////////////////
                            
                        printf("ID associato all' ordine: {{ %s }}", ID);
                        printf("\nSto elaborando l'ordine!\n");
                        //FullWrite per passare ID al server C
                        write(connfd, ID, sizeof(ID));
                        printf("\nInvio ID al ServerCentrale \n");
                        close(connfd); 
                            
                            
                            while(1)
                                {   //  ACCEPT : accetta la connessione con il Client
                                    client_connfd = Accept(client_listenfd, (struct sockaddr *) NULL, NULL);

                        			read(client_connfd,&pagato,sizeof(pagato));
                        			printf("Preparazione ordine in corso....\n");
                                    //Sleep per simulare la preparazione del pasto.
                        			sleep(orario_int);

                        			int pronto=1;

                        			write(client_connfd,&pronto, sizeof(pronto));
                        			printf("Ordine pronto\n");
                        			
                        			char scontrino[24];
                        			
                                    //Ricevo di nuovo l' ID come uno scontrino per controllare che sia uguale.
                        			read(client_connfd,scontrino, sizeof(scontrino));
                        			
                                    int ritirato=0;
                                    
                        			if(pagato==1 && (strcmp(ID,scontrino)==0))
            		        			{
            		        				ritirato=1;
            		        				write(client_connfd, &ritirato, sizeof(ritirato));
            		        				printf("Ordine ritirato da %s \n", matricola);
            		        			}
            		        		else
            		        			{
            		        			    ritirato=2;
            		        			    write(client_connfd, &ritirato, sizeof(ritirato));
            		        			    printf("ID sbagliato!");
            		        			    
            		        			}
                        			
                        		    close(client_connfd);
                                    break;
                                }
                                exit(0);
                    }
                                else//padre
                                    {
                                        close(connfd);
                                    }
                                    
                    
                    }
                    exit(0);
            	}
        
        
    if(strcmp(argv[2],"viaacton")==0)
        {
            while(1)
            	{	    
            	    printf("\n\t****SERVER GESTIONE SEDI CENTRO VIA ACTON****\n");
            	    //  ACCEPT : accetta la connessione con il Server Centrale
            	    connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);
            	    
                    if((pid=fork())<0)//Effettuo Fork
                        { 
                            perror("errore nella fork()");
                            exit(-1);
                        }
                    if(pid==0)//figlio
                    {
                        close(listenfd);
                        //read(connfd, sede, sizeof(sede));//Ricevo il nome della sede scelto dal Client
                        strcpy(sedescelta, "ViaActon");
                        
                        
                        //Ricevo il resto dell' ordine
                        read(connfd, &orario, sizeof(orario));
                        read(connfd, matricola, sizeof(matricola));                
                        read(connfd, utente_menu, sizeof(utente_menu));
                        orario_int=atoi(orario);
            
                        printf("- Matricola Utente: %s\n- Menu Scelto: %s \n- Ora: %s \n", matricola, utente_menu, orario);
                        printf("Genero ID Ordine....\n");
                            
                        //Genero ID///////////////////////////////
            		    int prefisso_int=200;
            		    char ID[24];
            		    sprintf(ID, "%d", prefisso_int);
            		    srand(time(NULL));
            		    int codice_int;    
            		    codice_int=rand() % 100 + 1;
            		    char codice_char[4];
            		    sprintf(codice_char, "%d", codice_int);
            		    strcat(ID,codice_char);
            		    ///////////////////////////////////////////
                            
                        printf("ID associato all' ordine: {{ %s }}", ID);
                        printf("\nSto elaborando l'ordine!\n");
                        //FullWrite per passare ID al server C
                        write(connfd, ID, sizeof(ID));
                        printf("\nInvio ID al ServerCentrale \n");
                        close(connfd);  
                            
                            
                             while(1)
                                {   //  ACCEPT : accetta la connessione con il Client
                                    client_connfd = Accept(client_listenfd, (struct sockaddr *) NULL, NULL);
                                    
                                    
                                    
                        			read(client_connfd,&pagato,sizeof(pagato));
                        			printf("Preparazione ordine in corso....\n");
                                    //Sleep per simulare la preparazione del pasto.
                        			sleep(orario_int);
                        			int pronto=1;
                        			write(client_connfd,&pronto, sizeof(pronto));
                        			printf("Ordine pronto\n");
                        			char scontrino[24];
                        			
                                    //Ricevo di nuovo l' ID come uno scontrino per controllare che sia uguale.
                        			read(client_connfd,scontrino, sizeof(scontrino));
                        			
                                    int ritirato=0;
                        		    if(pagato==1 && (strcmp(ID,scontrino)==0))
            		        			{
            		        				ritirato=1;
            		        				write(client_connfd, &ritirato, sizeof(ritirato));
            		        				printf("Ordine ritirato da %s \n", matricola);
            		        			}
            		        	    else
            		        			{
            		        			    ritirato=2;
            		        			    write(client_connfd, &ritirato, sizeof(ritirato));
            		        			    printf("ID sbagliato!");
            		        			    
            		        			}
                        			
                        		    close(client_connfd);
                                    break;
                                }
                                exit(0);
                    }
                                else//padre
                                    {
                                        close(connfd);
                                    }
                                    
                    
                    }
                    exit(0);
            	}
        
            
        
    if(strcmp(argv[2],"saporicampani")==0)
        {
            while(1)
            	{	    
            	    printf("\n\t****SERVER GESTIONE SEDI SAPORI CAMPANI****\n");
            	    //  ACCEPT : accetta la connessione con il Server Centrale
            	    connfd = Accept(listenfd, (struct sockaddr *) NULL, NULL);
            	    
                    if((pid=fork())<0)//Effettuo Fork
                        { 
                            perror("errore nella fork()");
                            exit(-1);
                        }
                    if(pid==0)//figlio
                    {
                        close(listenfd);
                        //read(connfd, sede, sizeof(sede));//Ricevo il nome della sede scelto dal Client
                        strcpy(sedescelta, "SaporiCampani");

                        //Ricevo il resto dell' ordine
                        read(connfd, &orario, sizeof(orario));
                        read(connfd, matricola, sizeof(matricola));                
                        read(connfd, utente_menu, sizeof(utente_menu));
                        orario_int=atoi(orario);
            
                        printf("- Matricola Utente: %s\n- Menu Scelto: %s \n- Ora: %s \n", matricola, utente_menu, orario);
                        printf("Genero ID Ordine....\n");
                            
                        //Genero ID///////////////////////////////
            		    int prefisso_int=300;
            		    char ID[24];
            		    sprintf(ID, "%d", prefisso_int);
            		    srand(time(NULL));
            		    int codice_int;    
            		    codice_int=rand() % 100 + 1;
            		    char codice_char[4];
            		    sprintf(codice_char, "%d", codice_int);
            		    strcat(ID,codice_char);
            		    ///////////////////////////////////////////
                            
                        printf("ID associato all' ordine: {{ %s }}", ID);
                        printf("\nSto preparando l'ordine!\n");
                        //Write per passare ID al server C
                        write(connfd, ID, sizeof(ID));
                        printf("\nInvio ID al ServerCentrale \n");
                        close(connfd); 
                            
                            
                             while(1)
                                {   //  ACCEPT : accetta la connessione con il Client
                                    client_connfd = Accept(client_listenfd, (struct sockaddr *) NULL, NULL);
                                    
                                    read(client_connfd,&pagato,sizeof(pagato));
                                    
                        		    printf("Preparazione ordine in corso....\n");
                        		    
                                    //Sleep per simulare la preparazione del pasto.
                        			sleep(orario_int);
                        			
                        			int pronto=1;
                        			write(client_connfd,&pronto, sizeof(pronto));
                        			printf("Ordine pronto\n");
                        			char scontrino[24];
                        			
                                    //Ricevo di nuovo l' ID come uno scontrino per controllare che sia uguale.
                        			read(client_connfd,scontrino, sizeof(scontrino));
                                    int ritirato=0;
                        			    if(pagato==1 && (strcmp(ID,scontrino)==0))
            		        			{
            		        				ritirato=1;
            		        				write(client_connfd, &ritirato, sizeof(ritirato));
            		        				printf("Ordine ritirato da %s \n", matricola);
            		        			}
            		        			else
            		        			{
            		        			    ritirato=2;
            		        			    write(client_connfd, &ritirato, sizeof(ritirato));
            		        			    printf("ID sbagliato!");
            		        			    
            		        			}
                        		    close(client_connfd);
                                    break;
                                }
                                exit(0);
                    }
                                else//padre
                                    {
                                        close(connfd);
                                    }
                                    
                    
                    }
                    exit(0);
            	}
	exit(0);
}
