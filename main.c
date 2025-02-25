#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    // Inizializza l'albero come vuoto
    Node *root = NULL;

    // Inserisci valori nel BST
    root = insertNode(root, 10);
    root = insertNode(root, 5);
    root = insertNode(root, 20);
    root = insertNode(root, 3);
    root = insertNode(root, 7);
    root = insertNode(root, 15);
    root = insertNode(root, 30);

    // Stampa in-order per verificare l'ordine
    printf("\nStampa in-order dei nodi:\n");
    inorderTraversal(root);

    // Test di eliminazione
    printf("\nEliminazione del nodo con chiave 20:\n");
    root = deleteNode(root, 20);
    inorderTraversal(root);

    printf("\nEliminazione del nodo con chiave 5:\n");
    root = deleteNode(root, 5);
    inorderTraversal(root);

    printf("\nEliminazione del nodo con chiave 10:\n");
    root = deleteNode(root, 10);
    inorderTraversal(root);

    // Libera la memoria allocata
    freeTree(root);

    return 0;
}
