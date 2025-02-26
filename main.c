#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{

    NodeLink root = NULL;

    // Carica l'albero da file
    loadTreeFromFile(&root, "database.txt");

    // Stampa immediata dopo la deserializzazione
    printf("Albero caricato da file:\n");
    inorderTraversal(root);
    printf("\n");

    /* Inseriamo valori di tipo int */
    Value intValue1 = {.type = TYPE_INT, .data.intValue = 10};
    Value intValue2 = {.type = TYPE_INT, .data.intValue = 5};
    Value intValue3 = {.type = TYPE_INT, .data.intValue = 20};

    root = insertNode(root, intValue1);
    root = insertNode(root, intValue2);
    root = insertNode(root, intValue3);

    /* Inseriamo valori di tipo string */
    Value stringValue1 = {.type = TYPE_STRING, .data.stringValue = "alpha"};
    Value stringValue2 = {.type = TYPE_STRING, .data.stringValue = "beta"};
    Value stringValue3 = {.type = TYPE_STRING, .data.stringValue = "charlie"};

    root = insertNode(root, stringValue1);
    root = insertNode(root, stringValue2);
    root = insertNode(root, stringValue3);

    /* Inserisci valori di tipo bool */
    Value boolValue1 = {.type = TYPE_BOOL, .data.boolValue = true};
    Value boolValue2 = {.type = TYPE_BOOL, .data.boolValue = false};

    root = insertNode(root, boolValue1);
    root = insertNode(root, boolValue2);

    // Esegui le operazioni di inserimento
    Value intValue = {.type = TYPE_INT, .data.intValue = 30};
    root = insertNode(root, intValue);

    // Stampa per verifica
    printf("Albero attuale:\n");
    inorderTraversal(root);

    // Salva l'albero su file prima di uscire
    FILE *file = fopen("database.txt", "w");
    if (file != NULL)
    {
        saveTreeToFile(root, file);
        fclose(file);
    }
    else
    {
        printf("Errore nell'apertura del file di salvataggio.\n");
    }

    // Libera la memoria
    freeTree(root);
    return 0;
}