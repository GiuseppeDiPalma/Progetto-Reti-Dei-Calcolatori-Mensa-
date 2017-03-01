#include "wrapper.h"

int main(int argc, char **argv)
{
    int listenfd, connfd;
    int sedefd, sedeconnfd;
    char nome[10], sede[500], buffer[500], utente[11];
    int cont=1;
	char orario[3];
    
    
    char buff_sede[500], buff_utente[11], buff_menu[50];
    
    
    struct sockaddr_in servaddr; //client
    struct sockaddr_in sedeaddr; //sede
    
    // PID usata per gestire la fork() e quindi la concorrenza
    pid_t pid;
    char costo[5]="5";
    char menupadre[]="Risotto alla pescatora;";
    char menufiglio[100];
    
// Famiglia di connessione per permettere la connessione con client e admin.
//______________________________________________________________________________
    listenfd = Socket(AF_INET, SOCK_STREAM, 0); //socket per conn del client
    // - famiglia AF_INET IPv4 Internet Protocols
    // - tipo SOCK_STREAM 
    // - 0 protocollo
    servaddr.sin_family = AF_INET;
    // - L' applicazione accettera connessioni
    //   da qualsiasi indirizzo associato al server.
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    // - htons memorizza nel campo sin_port della struct sedeaddr
    //          un intero scritto in network order.
    servaddr.sin_port = htons(PORTA);

    
    Bind(listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
    Listen(listenfd, PORTA);//mi metto in ascolto
//____________________________________________________________________________________

    while(1)
    {
        //printf("\n \t****SERVER****\n");
        //  ACCEPT : accetta la connessione con i client
        connfd = Accept(listenfd, (struct sockaddr *)NULL, NULL);  
        // Dichiarazione descrittore per la gestione della pipe()
        //   - fd[0] e' il file descriptor utilizzabile per la lettura.
        //   - fd[1] e' il file descriptor utilizzabile per la scrittura.
        // uso la pipe per far dialogare il figlio e il padre con un rapporto di
        // produttore/consumatore.
        int fd[2];
        
        if(pipe(fd)<0)
            {
                perror("pipe error");
                exit(1);
            }

        if((pid=fork())<0)//Fork
            { 
                perror("errore nella fork()");
                exit(-1);
            }
        // Con pid == 0 abbiamo la creazione di un processo figlio per ogni client
        // che si connette al server, creando così un server concorrente che permette
        // la gestione di più client connessi contemporaneamente. 
        if(pid==0)  //figlio
            {
                // Riceve da parte di un cliente o di un admin il proprio nome root
                // per decidere dove switchare la scelta e quindi con chi procedere
                read(connfd, nome, sizeof(nome));
                printf("\n (%d) - Si è connesso un: \t %s \n", cont, nome);
                close(fd[0]); 
                
                // nome == "admin" esegui admin e tutte le sue istruzioni
            if(strcmp(nome,"admin")==0)
                {
                        close(listenfd);
                        while(1)
                        {
                            printf("ADMIN CONNESSO\n");
                            // Legge il menu che l'admin ha modificato
                            read(connfd, menufiglio ,sizeof(menufiglio)); //leggo menu inserito
                            printf("**NOTIFCA. Ricevuto un nuovo menu: %s \n", menufiglio);
                            strcpy(menupadre,menufiglio);
                            // Scrive il menu con fd[1] inviandolo al padre.
                            write(fd[1], menupadre, sizeof(menupadre));
                            close(connfd);
                            break;
                        }
                }
            
            //nome == "client" esegui client generico e tutte le sue istruzioni
            else if(strcmp(nome,"client")==0)
                {
                        while(1)
                        {
                            
                            close(listenfd);
                            // legge la matricola inserita dal client
                            read(connfd, buff_utente, sizeof(buff_utente)); 
                            strcpy(utente, buff_utente); 
                            printf("Client con matricola: %s \n",utente);
                            
                            // scrive il menu al client
                            write(connfd, menupadre, sizeof(menupadre));
                            
                            sleep(2);
                            
                            
                            // invio costo
                            write(connfd, costo, sizeof(costo)); 
                            
                            
                            // riceve sede
                            read(connfd, buff_sede, sizeof(buff_sede));
                            strcpy(sede,buff_sede);
                            
                            //riceve orario
                            read(connfd, orario, sizeof(orario)); 
                          
                            //printf("buff_sede = %s\n", sede);
                            printf("\n\nOrdine Ricevuto: \n \t %s \n", menupadre);
                            printf("Da ritirare a: \n \t %s \n", sede);
                            printf("Alle ore: \n \t %s \n",orario);
                            int porta;
                            if(strcmp(sede,"CentroDirezionale")==0) { porta=3333; }
                            if(strcmp(sede,"ViaActon")==0) { porta=4444; }
                            if(strcmp(sede,"SaporiCampani")==0) { porta=5555; }
                            
                            // Famiglia di connessione per la connessione con un server sede   
                            //______________________________________________________________________________
                            sedefd = Socket(AF_INET, SOCK_STREAM, 0);
                            // - famiglia AF_INET IPv4 Internet Protocols
                        	// - tipo SOCK_STREAM 
                        	// - 0 protocollo
                            sedeaddr.sin_family = AF_INET;
                            // - htons memorizza nel campo sin_port della struct sedeaddr
                            //          un intero scritto in network order.
                            sedeaddr.sin_port = htons(porta);
    
                            sedeaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
                            
                            // - inet_pton converte la stringa passata come secondo argomento 
                            //             in un indirizzo di rete scritto in network order
                            //              e lo memorizza nella locazione di memoria puntata dal terzo argomento.
    						    if(inet_pton(AF_INET,"127.0.0.1", &sedeaddr.sin_addr) <= 0)
           						{
             					   fprintf(stderr,"inet_pton error for %s\n", "127.0.0.1"); 
             					   exit (1);
    						        }
                            //______________________________________________________________________________

                            // Connessione al server sede
                            Connect(sedefd,(struct sockaddr *) &sedeaddr, sizeof(sedeaddr));
                            
                            while(1)
                            {
                                //OPERAZIONI CON IL SERVER SEDE////////////////////////
                                // Invio nome della sede scelta al server per la gestione delle sedi
                                // così da sapere in quale sede indirizzare l'ordine.
                                //write(sedefd, sede, sizeof(sede));
                                
                                // Comunico alla sede scelta dal cliente poi, l'ordine completo.
                                write(sedefd, orario, sizeof(orario));
                                write(sedefd, utente, sizeof(utente));  
                                write(sedefd, menupadre, sizeof(menupadre));
                                
                                char ID[24];
                                
                                // Leggo l'id che mi manda la sede
                                read(sedefd, ID, sizeof(ID));
                                //printf("dalla sede arrivato ID: %s \n", ID);
                                // Invio l'id al client
                                write(connfd, ID, sizeof(ID));
                                //printf("invio id al client: %s \n",ID);
                                
                                close(connfd);
                                break;
                             }
                                close(sedefd);
                                
                                break;
                        }
                    break;
                }
                //Uccisione del figlio
                exit(0);
                
            }
        else//padre
        {
            // Chiudo connessione.
            close(connfd);
            // Chiudo fd[1] per la scrittura
            close(fd[1]); 
            
            // Ricezione da pipe menu nuovo e aggiornamento menu.
            read(fd[0], menupadre, sizeof(menupadre));
            write(STDOUT_FILENO, menupadre, sizeof(menupadre));
            /* - STDOUT_FILENO: flusso di output 
            *   All'avvio del programma, i descrittori di file associati 
            *   con i flussi stdin, stdout sono rispettivamente  0 e 1.
            *   standard output, cioè il file su cui ci si aspetta debbano essere 
            *   inviati i dati in uscita
            */
        }
        cont++;
      close(connfd); 
    }
  exit(0);
}
