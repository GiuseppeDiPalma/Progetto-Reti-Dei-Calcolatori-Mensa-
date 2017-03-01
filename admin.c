#include "wrapper.h"

int main(int argc, char ** argv)
{
    int i, sockfd;
    char root[10]="admin";
    char sceltamenu, ID[11], nuovomenu[200];
    struct sockaddr_in add_admin;
    // Famiglia di connessione per permettere la connessione con il server centrale
//______________________________________________________________________
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);
    add_admin.sin_family = AF_INET;
    add_admin.sin_port = htons(PORTA);
    if (inet_pton(AF_INET, argv[1], &add_admin.sin_addr) < 0)
    {
        fprintf(stderr,"inet_pton error for %s\n", argv[1]);
        exit(1);
    }
//_______________________________________________________________________
    Connect(sockfd,(struct sockaddr *) &add_admin, sizeof(add_admin)); //connessione al server
    // Scrivo il mio nome al server Centrale, così che possa riconoscere lo stato 
    // di superutente.
    write(sockfd, root, sizeof(root)); //dico chi sono al server

    while(1)
    {
        // L'admin si connette e può decidere di cambiare il menu del giorno
        // 1: cambia menu;
        // 2: esci dal programma;
        printf("-ADMIN CONNESSO-\n Desideri aggiornare il menu??\n");
        printf("\t [1] - SI\n \t [2] - NO \n ");
        scanf("%c", &sceltamenu);
        getchar();      // svuota buffer della scanf
        if(sceltamenu=='1')
        {
            // Inserisce il nuovo menu
            printf("Scrivi il menu del giorno:\n");
            gets(nuovomenu);
            //getchar();
            printf("Hai sostituito il menu con: %s \n", nuovomenu);
            // Il nuovo menu viene spedito al server centrale che lo aggiornerà.
            write(sockfd, nuovomenu, sizeof(nuovomenu));//invio il nuovo menu
        }
        else exit(0);
        exit(1);
    }
}
