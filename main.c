#include "bst.h"
#include <stdio.h>
#include <stdlib.h>

int main()
{
    /* Creazione di un nodo con chiave 10 */
    NodeLink root = createNode(10);

    /* Verifico che il nodo sia stato creato correttamente */
    if (root != NULL)
    {
        printf("Nodo creato con successo\n");
        printf("Chiave del nodo: %d\n", root->key);
        printf("Figlio sx: %p\n", root->left);
        printf("Figlio dx: %p\n", root->right);
    }
    else
    {
        printf("Errore nella creazione del nodo!\n");
    }

    free(root);

    return 0;
}