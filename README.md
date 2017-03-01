# Progetto-Reti-Dei-Calcolatori-Mensa-

# Descrizione del progetto

Si vuole realizzare una piattaforma per la gestione degli ordini di una mensa universitaria.
Il sistema si compone delle seguenti entità: ServerC, ServerS, Client, Admin. Il client
rappresenta un’interfaccia per l’utente attraverso cui è possibile inviare un ordine. Il client
si connette al ServerC dal quale riceve il menu del giorno. L’utente decide se effettuare un
ordine ed in caso affermativo, la sede (ServerS) e l’ora dove ritirarlo ed infine paga
ricevendo l’ID dell’ordine. Il client riceve un messaggio dalla sede prescelta (ServerS)
quando l’ordine è pronto. L’utente ritira l’ordine fornendo l’ID ed il ServerS rimuove
l’ordine. Attraverso l’admin è possibile aggiornare il menu memorizzato nel ServerC. Il
ServerC deve consentire la gestione di più ordini contemporaneamente.


# Descrizione e schemi dell’architettura
Il progetto è implementato per funzionare su piattaforma Unix/Linux secondo lo standard
Posix. L'intercomunicazione tra i processi avviene tramite Socket di Berkeley. Il protocollo
scelto è TCP, più affidabile grazie al canale di comunicazione che si crea tra i processi
client/server.


![alt tag](http://imageshack.com/a/img924/5885/0uO5lj.png)


# Descrizione e schemi del protocollo applicazione
Conversazione tra CLIENT||SERVER_CENTRALE||SEDE_SCELTA
![alt tag](http://imageshack.com/a/img924/8306/YwsJWP.png)
#
Conversazione tra ADMIN||SERVER_CENTRALE"
![alt tag](http://imageshack.com/a/img924/1688/nWWg0v.png)

## Dettagli in particolare
# Server Centrale
Il Server C accetta la connessione da parte dei client, riceve la matricola dal
client, invia il menù e costo, riceve gli ordini(sede e orario) che vengono inoltrati al Server
S con matricola dell’ utente e menù. Il Server C riceve un ID dell’ordine che viene inoltrato
al client che ha effettuato l’ordine. Per consentile la concorrenza di più Client il ServerC
effettua una fork. In caso di connessione con l’ Admin dal quale riceve un nuovo menu si
utilizza una Pipe tra il figlio e il padre generati dalla fork, per salvare il menu all’interno del
padre.
# Server Sede
Il Server S accetta connessione da parte del Server C, Il sistema simula la
presenza di più sedi (Centro Direzionale, Via Acton e Sapori Campani). Il Server S riceve l’
ordine (sede, orario, matricola e menù) dal Server C, risponde generando e inviando un ID
dell’ordine.
Il Server S accetta la connessione da parte del client, parte uno sleep che corrisponde all’
orario di ritiro ordine. Al termine dello sleep il ServerS invia una flag che segnala l’ ordine
pronto. Riceve l’ ID digitato da parte del client, se quest’ ultimo combacia all’ ID generato
setto la flag che segnala l’ ordine ritirato a 1, se non combacia a 2, e viene mandata al
client.
# Client
Il Client si connette al ServerC invia il root, per farsi riconoscere come client, e
la matricola. Riceve il menu e il costo. Il Client invia sede e ora dove ritirare il pasto.
Riceve dal ServerC l’ ID dell’ ordine effettuato e si connette al ServerS. Invia al ServerS
una flag che indica il pagamento effettuato. Riceve una flag che lo avvisa che l’ ordine è
pronto. Per ritirare il pasto l’ utente deve digitare l’ ID che ha ricevuto in precedenza, lo
invia al ServerS, e riceve una flag che comunica che il ritiro sia stato effettuato con
successo o meno.
# Admin
L’Admin si connette al ServerC e mediante una scelta, può scegliere di inserire o meno un 
nuovo menu, questo nuovo menu verrà inviato al serverC e dunque aggiornato, portando all'eliminazione
del precedente menu.

