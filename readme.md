# Binary Search Tree (BST) in C

*Approccio iniziale:* Questo progetto implementa una semplice struttura dati **Binary Search Tree (BST)** in linguaggio **C (C99)**. Il BST permette di inserire, cercare, eliminare e stampare elementi in ordine crescente.
**Approccio db relazionale:** todo
## ✅ Requisiti

- Compilatore GCC
- Supporto per lo standard C99
- Make (per automatizzare la compilazione e l'esecuzione)

## 📦 Installazione e Compilazione

1. **Clona il repository (se applicabile):**

   ```bash
   git clone <repository-url>
   cd <nome-cartella-progetto>
   ```

2. **Compilazione del progetto:**
   Per compilare l'intero progetto:

   ```bash
   make
   ```

   Questo comando genererà l'eseguibile `bst_test`.

3. **Esecuzione del programma:**
   Dopo aver compilato, puoi eseguire direttamente il programma:

   ```bash
   ./bst_test
   ```

4. **Compilazione ed esecuzione diretta:**
   Se vuoi compilare ed eseguire tutto con un solo comando:

   ```bash
   make run
   ```

5. **Pulire i file generati:**
   Per eliminare l'eseguibile e i file oggetto generati durante la compilazione:

   ```bash
   make clean
   ```

6. **Ricompilare tutto da zero:**
   Per eliminare i file generati e ricompilare completamente il progetto:
   ```bash
   make rebuild
   ```

## ⚙️ Comandi disponibili

| Comando        | Descrizione                                 |
| -------------- | ------------------------------------------- |
| `make`         | Compila il progetto e genera l'eseguibile   |
| `make run`     | Compila ed esegue direttamente il progetto  |
| `make clean`   | Rimuove tutti i file oggetto e l'eseguibile |
| `make rebuild` | Pulisce e ricompila tutto da zero           |

## 📖 Funzionalità implementate

- Creazione di un nodo nel BST
- Inserimento di un nuovo elemento
- Ricerca di un elemento
- Eliminazione di un elemento
- Stampa degli elementi in ordine crescente

## 👨‍💻 Come contribuire

1. Fai un fork del progetto.
2. Crea un nuovo branch: `git checkout -b feature/nuova-funzionalita`
3. Aggiungi le tue modifiche.
4. Fai un commit: `git commit -m 'Aggiunta nuova funzionalità'`
5. Fai un push sul tuo branch: `git push origin feature/nuova-funzionalita`
6. Crea una pull request.

## 📄 Licenza

Questo progetto è rilasciato sotto la licenza MIT.
