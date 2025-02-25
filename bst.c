#include <stdio.h>
#include <stdlib.h>
#include "bst.h"

/* Funzione per creare un nuovo nodo*/
NodeLink createNode(int key)
{
    /* Allochiamo memoria per un nuovo nodo */
    NodeLink newNode = (NodeLink)malloc(sizeof(Node));
    /* Controllo se l'allocazione ha avuto successo */
    if (newNode == NULL)
    {
        printf("Errore: memoria non disponibile\n");
        /* Usciamo dal programma in caso di errore */
        exit(1);
    }
    /* Assegnamo il valore al nodo */
    newNode->key = key;
    /* Nessun figlio sinistro/destro inizialmente*/
    newNode->left = NULL;
    newNode->right = NULL;
    /* Restituiamo il puntatore al nodo */
    return newNode;
}