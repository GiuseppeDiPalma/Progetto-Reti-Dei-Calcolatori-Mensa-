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
"Conversazione" tra CLIENT||SERVER_CENTRALE||SEDE_SCELTA
![alt tag](http://imageshack.com/a/img924/8306/YwsJWP.png)

"Conversazione" tra ADMIN||SERVER_CENTRALE"
![alt tag](http://imageshack.com/a/img924/1688/nWWg0v.png)

