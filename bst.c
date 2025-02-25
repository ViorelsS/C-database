#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bst.h"

static int currentId = 0;

/* -----------------------------------------*/
/* --------------- SUPPORTO ----------------*/
/* -----------------------------------------*/

char *v_strdup(const char *s)
{
    char *copy = malloc(strlen(s) + 1);
    if (copy)
    {
        strcpy(copy, s);
    }
    return copy;
}

/* -----------------------------------------*/
/* --------------- DATABASE ----------------*/
/* -----------------------------------------*/

/* Funzione per confrontare due chiavi generiche */

int compareValues(Value a, Value b)
{
    if (a.type != b.type)
    {
        /* Ordiniamo i tipi diversi secondo una gerarchia arbitraria */
        return a.type -
               b.type;
    }

    switch (a.type)
    {
    case TYPE_INT:
        return a.data.intValue - b.data.intValue;
    case TYPE_STRING:
        return strcmp(a.data.stringValue, b.data.stringValue);
    case TYPE_BOOL:
        return a.data.boolValue - b.data.boolValue;
    default:
        return 0;
    }
}

/* Funzione per creare un nuovo nodo*/
NodeLink createNode(Value key, int id)
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

    /* Assegnamo l'ID auto-incrementale*/
    newNode->id = id;

    /* Copiamo il valore in base al tipo*/
    newNode->key.type = key.type;
    switch (key.type)
    {
    case TYPE_INT:
        newNode->key.data.intValue = key.data.intValue;
        break;
    case TYPE_STRING:
        /* Copia sicura della stringa */
        newNode->key.data.stringValue = v_strdup(key.data.stringValue);
        if (newNode->key.data.stringValue == NULL)
        {
            printf("Errore: memoria non disponibile per la stringa!\n");
            exit(1);
        }
        break;
    case TYPE_BOOL:
        newNode->key.data.boolValue = key.data.boolValue;
        break;
    }

    newNode->left = NULL;
    newNode->right = NULL;

    /* Restituiamo il puntatore al nodo */
    return newNode;
}

/* Funzione per aggiungere un nodo */
NodeLink insertNode(NodeLink root, Value key)
{
    /* Caso base: se l'albero è vuoto, creiamo un nuovo nodo*/
    if (root == NULL)
    {
        return createNode(key, currentId++);
    }

    int cmp = compareValues(key, root->key);

    /* Se la chiave è minore, inserisci a sinistra */
    if (cmp < 0)
    {
        root->left = insertNode(root->left, key);
    }

    /* Se la chiave è maggiore, inserisci a destra */
    else if (cmp > 0)
    {
        root->right = insertNode(root->right, key);
    }

    /* Se la chiave è giù presente, non fare nulla (no duplicati) */

    /* Restituiamo il nodo radice aggiornato*/
    return root;
}

/* Funzione di stampa del nodo in base al tipo */
void printValue(Value value)
{
    switch (value.type)
    {
    case TYPE_INT:
        printf("%d", value.data.intValue);
        break;
    case TYPE_STRING:
        printf("%s", value.data.stringValue);
        break;
    case TYPE_BOOL:
        printf("%s", value.data.boolValue ? "true" : "false");
        break;
    }
}

/* Funzione per stampare i nodi in ordine crescente */
void inorderTraversal(NodeLink root)
{
    if (root != NULL)
    {
        /* Visita il sottoalbero sinistro */
        inorderTraversal(root->left);
        /* Stampa della chiave del nodo corrente */
        printValue(root->key);
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

        // Libera il valore se è una stringa
        if (root->key.type == TYPE_STRING && root->key.data.stringValue != NULL)
        {
            free(root->key.data.stringValue);
        }

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
NodeLink deleteNode(NodeLink root, Value key)
{
    /* Caso base: se l'albero è vuoto */
    if (root == NULL)
    {
        return root;
    }

    int cmp = compareValues(key, root->key);

    /*  Se la chiave è minore, andiamo a sinistra*/
    if (cmp < 0)
    {
        root->left = deleteNode(root->left, key);
    }
    /* Se la chiave è maggiore, andiamo a destra */
    else if (cmp > 0)
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

/* Funzione di ricerca di un valore nell'albero */
NodeLink searchNodeByKey(NodeLink root, Value key)
{
    /* Caso base: albero vuoto o nodo trovato */
    if (root == NULL)
    {
        return NULL;
    }

    int cmp = compareValues(key, root->key);

    /* Se la chiave è minore, cerca a sinistra */
    if (cmp == 0)
    {
        return root;
    }
    else if (cmp < 0)
    {
        return searchNodeByKey(root->left, key);
    }
    /* Se la chiave è maggiore, cerca a destra*/
    else
    {
        return searchNodeByKey(root->right, key);
    }
}

/* Funzione di ricerca di un nodo per ID */
NodeLink searchNodeById(NodeLink root, int id)
{
    /* Caso base: se l'albero è vuoto o troviamo il nodo */
    if (root == NULL)
    {
        return NULL;
    }

    if (root->id == id)
    {
        return root;
    }

    /* Cerca ricorsivamente nel sottoalbero sinistro */
    NodeLink found = searchNodeById(root->left, id);
    if (found != NULL)
    {
        return found;
    }

    /* Se non è stato trovato a sinistra, cerchiamo a destra */
    return searchNodeById(root->right, id);
}