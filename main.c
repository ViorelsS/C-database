#include "bst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    /* Inizializziamo l'albero vuoto */
    NodeLink root = NULL;

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

    /* Stampa in-order per verificare l'ordine */
    printf("\nStampa in-order dei nodi:\n");
    inorderTraversal(root);

    /* Ricerca per ID (proviamo con l'ID 3) */
    int searchId = 3;
    NodeLink foundNode = searchNodeById(root, searchId);
    if (foundNode != NULL)
    {
        printf("\nNodo con ID %d trovato: ", foundNode->id);
        printValue(foundNode->key);
        printf("\n");
    }
    else
    {
        printf("\nNodo con ID %d non trovato.\n", searchId);
    }

    /* Liberiamo la memoria allocata */
    freeTree(root);
    return 0;
}
