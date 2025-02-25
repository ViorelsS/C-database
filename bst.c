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

/* PER L'ELIMINAZIONE DI UN NODO CI SONO 3 CASI DA GESTIRE
 * 1. Nodo senza figli (foglia): basta rimuoverlo e aggiornare il puntatore del genitore a NULL
 * 2. Nodo con un solo figlio: il nodo viene rimosso e il figlio prende il suo posto
 * 3. Nodo con due figli:
 *              - Troviamo il successore in-order (il nodo con il valore minimo nel sottoalbero destro)
 *              - Sostituiamo la chiave del nodo da eliminare con quella del successore.
 *              - Eliminiamo il successore dal sottoalbero destro.
 */

/* Funzione per trovare il nodo con il valore minimo in un albero */
NodeLink findMin(NodeLink node)
{
    while (node->left != NULL)
    {
        node = node->left;
    }
    return node;
}

/* Funzione per eliminare un nodo dall'albero */
NodeLink deleteNode(NodeLink root, int key)
{
    /* Caso base: se l'albero è vuoto */
    if (root == NULL)
    {
        return root;
    }

    /*  Se la chiave è minore, andiamo a sinistra*/
    if (key < root->key)
    {
        root->left = deleteNode(root->left, key);
    }
    /* Se la chiave è maggiore, andiamo a destra */
    else if (key > root->key)
    {
        root->right = deleteNode(root->right, key);
    }
    /* Se la chiave è uguale, elimina il nodo */
    else
    {
        /* Caso 1: nodo senza figli */
        if (root->left == NULL && root->right == NULL)
        {
            free(root);
            return NULL;
        }
        /* Caso 2: nodo con un solo figlio */
        else if (root->left == NULL)
        {
            NodeLink tmp = root->right;
            free(root);
            return tmp;
        }
        else if (root->right == NULL)
        {
            NodeLink tmp = root->left;
            free(root);
            return tmp;
        }
        /* Caso 3: Nodo con due figli */
        else
        {
            /* Successore in order */
            NodeLink tmp = findMin(root->right);
            /* Copia il valore del successore */
            root->key = tmp->key;
            /* Elimina il successore */
            root->right = deleteNode(root->right, tmp->key);
        }
    }

    return root;
}