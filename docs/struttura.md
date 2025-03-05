```bash
/v-database
│── /include              # Header files (.h)
│   │── types.h           # Definizione dei tipi di dato
│   │── storage.h         # BST o altro storage per le righe
│   │── table.h           # Struttura e funzioni per le tabelle
│   │── row.h             # Struttura e funzioni per le righe
│   │── relation.h        # Gestione delle relazioni tra le tabelle
│   │── db.h              # API principale del database
│── /src                  # Implementazioni (.c)
│   │── storage.c         # Implementazione del BST o altra struttura
│   │── table.c           # Implementazione delle tabelle
│   │── row.c             # Implementazione delle righe
│   │── relation.c        # Implementazione delle relazioni
│   │── db.c              # Implementazione delle API del database
│── /tests                # Test unitari
│   │── test_db.c         # Test per il database
│── main.c                # Punto di ingresso del programma
│── Makefile              # Per compilare tutto facilmente
```
