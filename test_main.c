#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void testDeserialization();
void testInsertionAndOrder();
void testSearch();
void testDeletion();
void testStringMemoryManagement();
void testStressMemoryManagement();
void inorderLimited(NodeLink node, int *count);

int main()
{
    /* TEST AUTOMATICO */
    testDeserialization();
    testInsertionAndOrder();
    testSearch();
    testDeletion();
    testStringMemoryManagement();
    testStressMemoryManagement();

    return 0;
}

/* Funzione di test per la deserializzazione */
void testDeserialization()
{
    FILE *file = fopen("database_test.txt", "w");
    if (file == NULL)
    {
        printf("Errore: impossibile creare il file di test.\n");
        return;
    }

    fprintf(file, "0,TYPE_INT,42\n");
    fprintf(file, "1,TYPE_STRING,hello\n");
    fprintf(file, "2,TYPE_BOOL,true\n");
    fclose(file);

    /* Carichiamo i dati nel BST */
    NodeLink root = NULL;
    loadTreeFromFile(&root, "database_test.txt");

    /* Stampiamo l'albero per verificare il contenuto */
    printf("\n--- Test deserializzazione ---\n");
    printf("Dati caricati dall'albero di test:\n");
    inorderTraversal(root);
    printf("\n--- Fine Test ---\n");

    /* Liberiamo l'albero */
    freeTree(root);
}

/* Funzione di test per l'inserimento e l'ordine */
void testInsertionAndOrder()
{
    NodeLink root = NULL;

    /* Inseriamo i dati di test */
    Value val1 = {.type = TYPE_INT, .data.intValue = 20};
    Value val2 = {.type = TYPE_INT, .data.intValue = 10};
    Value val3 = {.type = TYPE_INT, .data.intValue = 30};

    root = insertNode(root, val1);
    root = insertNode(root, val2);
    root = insertNode(root, val3);

    /* Stampa in-order (dovrebbe essere 10, 20, 30) */
    printf("\n--- Test Inserimento e Ordine ---\n");
    printf("L'albero in ordine dovrebbe stampare: 10 20 30\n");
    inorderTraversal(root);
    printf("\n--- Fine Test ---\n");

    freeTree(root);
}

/* Funzione di test per la ricerca di nodi */
void testSearch()
{
    NodeLink root = NULL;

    /* Inseriamo dati di test */
    Value val1 = {.type = TYPE_STRING, .data.stringValue = "banana"};
    Value val2 = {.type = TYPE_STRING, .data.stringValue = "apple"};
    Value val3 = {.type = TYPE_STRING, .data.stringValue = "cherry"};

    root = insertNode(root, val1);
    root = insertNode(root, val2);
    root = insertNode(root, val3);

    /* Cerchiamo un nodo esistente */
    printf("\n--- Test Ricerca Nodo ---\n");
    NodeLink found = searchNodeByKey(root, val2);
    if (found)
    {
        printf("Nodo trovato: ");
        printValue(found->key);
        printf("\n");
    }
    else
    {
        printf("Errore: nodo non trovato!\n");
    }

    /* Cerchiamo un nodo inesistente */
    Value notPresent = {.type = TYPE_STRING, .data.stringValue = "orange"};
    found = searchNodeByKey(root, notPresent);
    if (found)
    {
        printf("Errore: il nodo non dovrebbe esistere!\n");
    }
    else
    {
        printf("Nodo 'orange' non trovato, test passato.\n");
    }

    printf("--- Fine Test ---\n");

    freeTree(root);
}

/* Funzione di test per l'eliminazione di nodi */
void testDeletion()
{
    NodeLink root = NULL;

    /*Inseriamo dati di test */
    Value val1 = {.type = TYPE_INT, .data.intValue = 15};
    Value val2 = {.type = TYPE_INT, .data.intValue = 10};
    Value val3 = {.type = TYPE_INT, .data.intValue = 20};

    root = insertNode(root, val1);
    root = insertNode(root, val2);
    root = insertNode(root, val3);

    /* Eliminiamo un nodo foglia (senza figli) */
    printf("\n--- Test Eliminazione Nodo (Foglia) ---\n");
    root = deleteNode(root, val2);
    printf("Dopo eliminazione del nodo con valore 10:\n");
    inorderTraversal(root);
    printf("\n--- Fine Test ---\n");

    freeTree(root);
}

/* Funzione per testare l'allocazione e deallocazione delle stringhe */
void testStringMemoryManagement()
{
    NodeLink root = NULL;

    printf("\n--- Test Gestione Memoria Stringhe ---\n");

    /* Creiamo stringhe allocate con v_strdup() */
    char *s1 = v_strdup("alpha");
    char *s2 = v_strdup("beta");
    char *s3 = v_strdup("gamma");

    /* Inseriamo nel BST */
    Value val1 = {.type = TYPE_STRING, .data.stringValue = s1};
    Value val2 = {.type = TYPE_STRING, .data.stringValue = s2};
    Value val3 = {.type = TYPE_STRING, .data.stringValue = s3};

    root = insertNode(root, val1);
    root = insertNode(root, val2);
    root = insertNode(root, val3);

    /* Le stringhe originali sono state duplicate, quindi possiamo liberarle */
    free(s1);
    free(s2);
    free(s3);

    /* Controlliamo che le stringhe siano state salvate */
    printf("Albero dopo inserimento di stringhe:\n");
    inorderTraversal(root);
    printf("\n");

    /* Libera la memoria */
    freeTree(root);

    /* Se Valgrind non trova memory leak, il test è passato */
    printf("✅ Test completato! Controlla Valgrind per verificare che non ci siano memory leak.\n");
    printf("--- Fine test ---\n");
}

/* Stress test */
void testStressMemoryManagement()
{
    NodeLink root = NULL;
    /* Inseriamo 10.000 nodi */
    int numNodes = 20;

    printf("\n--- Test stress gestione memoria stringhe ---\n");

    /* Inseriamo molte stringhe nel BST */
    for (int i = 0; i < numNodes; i++)
    {
        char buffer[20];
        snprintf(buffer, sizeof(buffer), "string-%d", i);
        Value val = {.type = TYPE_STRING, .data.stringValue = v_strdup(buffer)};
        root = insertNode(root, val);
        free(val.data.stringValue);
    }

    printf("✅ %d stringhe inserite nel BST.\n", numNodes);

    /* Eliminiamo casualmente alcuni nodi casualmente */
    srand(time(NULL));
    /* Rimuoviamo il 10% dei nodi*/
    for (int i = 0; i < numNodes / 10; i++)
    {
        int idToRemove = rand() % numNodes;
        char buffer[25];
        snprintf(buffer, sizeof(buffer), "string-%d", idToRemove);

        Value val = {.type = TYPE_STRING, .data.stringValue = v_strdup(buffer)};

        NodeLink nodeToDelete = searchNodeByKey(root, val);
        if (nodeToDelete != NULL && nodeToDelete->key.type == TYPE_STRING)
        {
            free(nodeToDelete->key.data.stringValue);
            nodeToDelete->key.data.stringValue = NULL;
        }

        /* Eliminiamo il nodo */
        root = deleteNode(root, val);

        free(val.data.stringValue);
    }

    printf("✅ Rimosse il 10%% delle stringhe casualmente.\n");

    /* Controlliamo che le stringhe siano ancora in ordine */
    printf("Stampa in-order dopo eliminazioni (prime 20 voci):\n");

    int count = 0;

    inorderLimited(root, &count);
    printf("\n");

    /* Liberiamo la memoria */
    freeTree(root);

    /* Se Valgrind non trova memory leak, il test è passato */
    printf("✅ Test completato! Controlla Valgrind per verificare che non ci siano memory leak.\n");
    printf("--- Fine test ---\n");
}

void inorderLimited(NodeLink node, int *count)
{
    if (node != NULL && *count < 20)
    {
        inorderLimited(node->left, count);
        printValue(node->key);
        printf(" ");
        (*count)++;
        inorderLimited(node->right, count);
    }
}