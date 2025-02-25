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
    printf("Stampa in-order dei nodi:\n");
    inorderTraversal(root);

    // Libera la memoria allocata
    freeTree(root);

    return 0;
}
