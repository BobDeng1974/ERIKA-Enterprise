Test 5

Il test5 riguarda l'attivazione di 2 tasks. Il primo
a priorit� pi� bassa � attivato nel main all'inizio mentre 
il secondo a priorit� pi� alta � attivato da un'interrupt (2)
innestata su un'altra interrupt (1) a priorit� pi� bassa.
Il test serve per verificare che lo scheduler sia chiamato
al termine dell'interrupt 1.