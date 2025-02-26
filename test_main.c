#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void testDeserialization();
void testInsertionAndOrder();
void testSearch();
void testDeletion();

int main()
{
    /* TEST AUTOMATICO */
    testDeserialization();
    testInsertionAndOrder();
    testSearch();
    testDeletion();

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