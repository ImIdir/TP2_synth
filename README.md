### Question 1: Utilisation des arguments en ligne de commande pour les programmes gettftp et puttftp

**Énoncé :**
Utilisez des arguments en ligne de commande pour obtenir des informations de demande (serveur et fichier) pour les programmes gettftp et puttftp.

**Approche :**
- Les programmes gettftp et puttftp prennent deux arguments en ligne de commande : le serveur et le fichier.
- Le fichier `fonction.h` définit les fonctions et constantes nécessaires.
- Les fichiers `gettftp.c` et `puttftp.c` vérifient et récupèrent les données des arguments en ligne de commande.
  
**Détails de l'implémentation :**
- Les programmes vérifient le nombre d'arguments saisis en ligne de commande.
- Ils utilisent `strtok` pour séparer le nom du serveur et le numéro de port.
- Si le numéro de port n'est pas fourni, le programme utilise le port par défaut (1069).

### Instructions pour l'utilisation :
1. Pour utiliser `gettftp` : `./gettftp servername:port filename`
2. Pour utiliser `puttftp` : `./puttftp servername:port filename`

### Question 2: Utilisation de `getaddrinfo` pour obtenir l'adresse du serveur

**Énoncé :**
Appelez `getaddrinfo` pour obtenir l'adresse du serveur.

**Approche :**
- La fonction `printServerAddress` du fichier `fonctions.c` utilise `getaddrinfo` pour récupérer l'adresse du serveur.
- Elle prend comme arguments le nom du serveur et le numéro de port.
- Les fichiers `fonction.h` et `fonction.c` définissent cette fonction.
- Le fichier `gettftp.c` utilise cette fonction pour afficher l'adresse du serveur à partir des arguments en ligne de commande.

**Détails de l'implémentation :**
- La fonction `printServerAddress` utilise `getaddrinfo` pour récupérer les informations d'adresse du serveur.
- Elle itère sur ces informations pour afficher l'adresse du serveur.
- Les détails d'adresse sont imprimés pour chaque résultat trouvé par `getaddrinfo`.

### Instructions pour l'utilisation :
1. Assurez-vous que `gettftp` est correctement compilé.
2. Utilisez `gettftp` avec la commande suivante : `./gettftp servername:port filename`

Le programme affichera "Try on server : servername on port port" suivi de "Server address: (adresse IP)". Cette adresse IP est récupérée en utilisant `getaddrinfo`.

### Question 3: Création d'un socket de connexion au serveur

**Énoncé :**
Créez un socket de connexion au serveur.

**Approche :**
- La fonction `reserveSocket` du fichier `fonctions.c` crée un socket de connexion au serveur.
- Elle prend comme arguments le nom du serveur et le numéro de port.
- Les fichiers `fonction.h` et `fonction.c` définissent cette fonction.
- Le fichier `gettftp.c` utilise cette fonction pour réserver un socket de connexion au serveur à partir des arguments en ligne de commande.

**Détails de l'implémentation :**
- La fonction `reserveSocket` utilise `getaddrinfo` pour récupérer les informations d'adresse du serveur.
- Elle crée un socket en itérant sur les informations d'adresse obtenues.
- Le socket créé est utilisé pour établir une connexion avec le serveur.

### Instructions pour l'utilisation :
1. Assurez-vous que `gettftp` est correctement compilé.
2. Utilisez `gettftp` avec la commande suivante : `./gettftp servername:port filename`

Le programme affichera "Try on server : servername on port port" suivi de "Server address: (adresse IP)". Ensuite, il affichera "Socket (numéro de socket)". Ce numéro de socket représente le socket créé pour la connexion au serveur.

### Question 4: Mise en place d'une demande de fichier (RRQ) et réception des paquets de données (DAT) avec accusés de réception (ACK)

**Énoncé :**
Pour `gettftp` :
a) Construire une demande de lecture (Read Request - RRQ) correctement formée et l'envoyer au serveur.
b) Recevoir un fichier composé d'un seul paquet de données (DAT) et de son accusé de réception (ACK).
c) Recevoir un fichier composé de plusieurs paquets de données (DAT) et de leurs accusés de réception respectifs (ACK).

**Changements apportés aux codes :**
- Dans `fonction.c`, des fonctions telles que `sendRRQ`, `receiveAndAcknowledge`, `sendAck`, `receiveOneBlock`, etc., ont été ajoutées ou modifiées pour répondre aux exigences des différentes étapes.
- Le fichier `gettftp.c` a été modifié pour appeler ces nouvelles fonctions et gérer le processus d'envoi de la demande RRQ et de réception des paquets de données avec leurs accusés de réception.

**Détails de l'implémentation :**
- La fonction `sendRRQ` construit une demande RRQ conforme et l'envoie au serveur.
- La fonction `receiveAndAcknowledge` reçoit un fichier composé d'un seul paquet de données et de son accusé de réception.
- Une boucle dans `gettftp.c` gère la réception de plusieurs paquets de données avec leurs accusés de réception correspondants.

### Instructions pour l'utilisation :
1. Assurez-vous que `gettftp` est correctement compilé.
2. Utilisez `gettftp` avec la commande suivante : `./gettftp servername:port filename`

Le programme exécutera les étapes de création de demande RRQ, de réception d'un seul paquet de données avec son accusé de réception, et de réception de plusieurs paquets de données avec leurs accusés de réception respectifs.

