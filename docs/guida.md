# 1. Creazione e gestione dei bst basici (con numeri)

# 2. Integrazione di un subset di tipi di dato

# 3. Modifica delle funzioni di base, integrando il supporto del tipo di valore (Value) e gestione per id

# 4. Controlliamo possibili memory leak con valgrind

```bash
valgrind --leak-check=full ./bst_test
```

# 5. Serializzazione dei dati -> formato testuale

Ogni riga rappresenterà un nodo:

```graphql
ID,TYPE,VALUE
0,TYPE_INT,10
1,TYPE_STRING,alpha
2,TYPE_BOOL,true

```

# 6. Serializzazione dei dati in binario
