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
Conversazione tra ADMIN||SERVER_CENTRALE
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

# Dettagli implementativi
# Server Centrale
Il codice del ServerC inizia con la creazione della Socket per la connessione con il Client
(o Admin). Entro nel While infinito e accetto la connessione da parte del Client.
Eseguo una fork per consentire la concorrenza del server.
Nel figlio la prima Write legge Root mandato da Client per capire se dialoga con un Admin
o un Client. Se entra nell’ If dell’Admin riceve il menu nuovo da Admin e lo passa tramite
una Pipe al padre dopo aver chiuso la Pipe in lettura.
Se entro nell’ If del Client con la prima Read leggo la matricola dell’utente. Con la
seguente Write mando al Client il menu del giorno e il costo. Con due Read ricevo ora e
sede di ritiro da aggiungere al menu, al costo e alla matricola dell’ utente che compongono
l’ordine. A questo punto cessa la connessione con il Client.
Mi connetto al ServerS con quattro Read invio menu, sede, orario, e matricola dell’ ordine.
Di risposta ricevo un ID dell ordine che inoltro con una Write al Client. A questo punto
cessa la connessione con il Client e con il ServerS.
Nel padre per prima cosa mi accerto che la connessione sia chiusa prima di fare altro.
Chiudo la Pipe in scrittura e ricevo il menu inviato dal figlio per aggiornarlo.
# Client
Il codice del Client inizia con la creazione della Socket per la connessione con il ServerC.
Dopo la Connect con una Write invio il root al server per farmi riconoscere in qualità di
client. La prima Write invia la matricola dell’utente, seguono due Read per la ricezione del
menu e del costo. All’interno di un While infinito si chiede all’utente se il menu è gradito,
quindi si decide se effettuare l’ordine. Viene chiesto in che sede ritirare il pasto e l’orario di
ritiro, e infine con due Write invio la sede e l’orario e riceve l’ ID dell’ ordine eseguito.
A questo punto il Client deve connettersi con il ServerS, effettuando una Connect su una
Socket in precedenza creata appositamente. Con una Read ricevo l’ID dell’ordine.
L’ utente paga impostando e inviando una flag al ServerS, quando l’ordine è pronto il
Client invia l’ID digitato dall’utente al ServerS , ricevo una flag impostata a 1 se l’ ID inviato
è stat corretto, altrimenti impostata a 2.
# Server Sedi
Il server delle sedi permette la gestione di tre sedi separate, questo grazia alla scrittura da
terminare del nome della sede, facendo così permette di scegliere fra tre sedi. Questo è
stato realizzato con l’uso del argv, e un If che effettua il controllo sui comandi da terminale.
Ogni sede compie le stesse operazioni:
Eseguo una fork per consentire la concorrenza del server.
Nel figlio accetto la connessione da parte del ServerC, legge con tre Read l’ordine
composto da orario, matricola e menù. Genero un ID e lo invio con una Write al ServerC,
subito dopo chiudo la connessione. Accetto la connessione da parte del Client, leggo la
flag che indica il pagamento effettuato, effettuo una Sleep per il tempo di attesa in
funzione dell’orario dell’ordine e invio la flag che indica lo stato pronto del pasto. Ricevo
l’ID comunicato dal Client. Se l’ordine risulta pagato e l’ID inviato dal Client combacia con
quello generato l’ordine viene considerato ritirato e viene inostrata l’informazione al Client.
Nel padre mi accerto di chiudere le connessioni.
##
# Istruzioni per la compilazione
##
La compilazione viene effettuata da terminale, con l'apertura in simultanea di almeno tre terminali 
nell'ordine di Server Centrale, Sede, Client (eventualmente tutte le sede richieste, eventualmente 
l'admin se occorre). 
I comandi sono:
bash serverC.sh
bash centrodirezionale.sh
bash client.sh
Eventualmente:
bash saporicampani.sh
bash viaacton.sh
bash admin.sh
