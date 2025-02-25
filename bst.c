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

/* Funzione per aggiungere un nodo */
NodeLink insertNode(NodeLink root, int key)
{
    /* Caso base: se l'albero è vuoto, creiamo un nuovo nodo*/
    if (root == NULL)
    {
        return createNode(key);
    }

    /* Se la chiave è minore, inserisci a sinistra */
    if (key < root->key)
    {
        root->left = insertNode(root->left, key);
    }

    /* Se la chiave è maggiore, inserisci a destra */
    else if (key > root->key)
    {
        root->right = insertNode(root->right, key);
    }

    /* Se la chiave è giù presente, non fare nulla (no duplicati) */

    /* Restituiamo il nodo radice aggiornato*/
    return root;
}

/* Funzione per stampare i nodi in ordine crescente */
void inorderTraversal(NodeLink root)
{
    if (root != NULL)
    {
        /* Visita il sottoalbero sinistro */
        inorderTraversal(root->left);
        /* Stampa della chiave del nodo corrente */
        printf("%d ", root->key);
        /* Visita il sottoalbero destro */
        inorderTraversal(root->right);
    }
}

/* Funzione per liberare la memoria allocata dall'albero */
void freeTree(NodeLink root)
{
    if (root != NULL)
    {
        /* Libera il sottoalbero sinistro */
        freeTree(root->left);
        /* Libera il sottoalbero destro */
        freeTree(root->right);
        /* Libera il nodo corrente */
        free(root);
    }
}