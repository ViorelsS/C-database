#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
        return (a.data.intValue > b.data.intValue) - (a.data.intValue < b.data.intValue);

    case TYPE_STRING:
        if (a.data.stringValue == NULL || b.data.stringValue == NULL)
        {
            return (a.data.stringValue == NULL) - (b.data.stringValue == NULL);
        }
        return strcmp(a.data.stringValue, b.data.stringValue);

    case TYPE_BOOL:
        return a.data.boolValue - b.data.boolValue;

    case TYPE_FLOAT:
        return (a.data.floatValue > b.data.floatValue) - (a.data.floatValue < b.data.floatValue);

    case TYPE_DOUBLE:
        return (a.data.doubleValue > b.data.doubleValue) - (a.data.doubleValue < b.data.doubleValue);

    case TYPE_DATE:
        if (a.data.dateValue.year != b.data.dateValue.year)
            return a.data.dateValue.year - b.data.dateValue.year;
        if (a.data.dateValue.month != b.data.dateValue.month)
            return a.data.dateValue.month - b.data.dateValue.month;
        return a.data.dateValue.day - b.data.dateValue.day;

    case TYPE_TIME:
        if (a.data.timeValue.hour != b.data.timeValue.hour)
            return a.data.timeValue.hour - b.data.timeValue.hour;
        if (a.data.timeValue.minute != b.data.timeValue.minute)
            return a.data.timeValue.minute - b.data.timeValue.minute;
        return a.data.timeValue.second - b.data.timeValue.second;

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
    case TYPE_FLOAT:
        newNode->key.data.floatValue = key.data.floatValue;
        break;
    case TYPE_DOUBLE:
        newNode->key.data.doubleValue = key.data.doubleValue;
        break;
    case TYPE_DATE:
        newNode->key.data.dateValue.year = key.data.dateValue.year;
        newNode->key.data.dateValue.month = key.data.dateValue.month;
        newNode->key.data.dateValue.day = key.data.dateValue.day;
        break;
    case TYPE_TIME:
        newNode->key.data.timeValue.hour = key.data.timeValue.hour;
        newNode->key.data.timeValue.minute = key.data.timeValue.minute;
        newNode->key.data.timeValue.second = key.data.timeValue.second;
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
    else
    {
        /* Sostituzione di una stringa esistente */
        if (key.type == TYPE_STRING)
        {

            /* Libera la vecchia stringa */
            free(root->key.data.stringValue);
            root->key.data.stringValue = v_strdup(key.data.stringValue);
        }
        else
        {
            /* Sostituzione diretta per INT e BOOL */
            root->key = key;
        }
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
        printf("%d ", value.data.intValue);
        break;
    case TYPE_STRING:
        printf("%s ", value.data.stringValue);
        break;
    case TYPE_BOOL:
        printf("%s ", value.data.boolValue ? "true" : "false");
        break;
    case TYPE_FLOAT:
        printf("%.2f ", value.data.floatValue);
        break;
    case TYPE_DOUBLE:
        printf("%.2lf ", value.data.doubleValue);
        break;
    case TYPE_DATE:
        printf("%04d-%02d-%02d ", value.data.dateValue.year,
               value.data.dateValue.month, value.data.dateValue.day);
        break;
    case TYPE_TIME:
        printf("%02d:%02d:%02d ", value.data.timeValue.hour,
               value.data.timeValue.minute, value.data.timeValue.second);
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
            root->key.data.stringValue = NULL;
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
        /* Liberiamo la stringa prima di eliminare il nodo */
        if (root->key.type == TYPE_STRING && root->key.data.stringValue != NULL)
        {
            free(root->key.data.stringValue);
            root->key.data.stringValue = NULL;
        }

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

            /* Se il nodo da eliminare contiene una stringa, liberiamo la memoria prima di sovrascrivere */
            if (root->key.type == TYPE_STRING && root->key.data.stringValue != NULL)
            {
                free(root->key.data.stringValue);
            }

            /* Copia il valore del successore */
            root->key.type = tmp->key.type;
            if (tmp->key.type == TYPE_STRING)
            {
                root->key.data.stringValue = v_strdup(tmp->key.data.stringValue);
            }
            else
            {
                root->key = tmp->key;
            }

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

/* Serializzazione */
void saveTreeToFile(NodeLink root, FILE *file)
{
    if (root != NULL)
    {
        // Scriviamo ID, tipo e valore
        fprintf(file, "%d,", root->id);
        switch (root->key.type)
        {
        case TYPE_INT:
            fprintf(file, "TYPE_INT,%d\n", root->key.data.intValue);
            break;
        case TYPE_STRING:
            fprintf(file, "TYPE_STRING,%s\n", root->key.data.stringValue);
            break;
        case TYPE_BOOL:
            fprintf(file, "TYPE_BOOL,%d\n", root->key.data.boolValue);
            break;
        case TYPE_FLOAT:
            fprintf(file, "TYPE_FLOAT,%f\n", root->key.data.floatValue);
            break;
        case TYPE_DOUBLE:
            fprintf(file, "TYPE_DOUBLE,%lf\n", root->key.data.doubleValue);
            break;
        case TYPE_DATE:
            fprintf(file, "TYPE_DATE,%04d-%02d-%02d\n",
                    root->key.data.dateValue.year, root->key.data.dateValue.month,
                    root->key.data.dateValue.day);
            break;
        case TYPE_TIME:
            fprintf(file, "TYPE_TIME,%02d:%02d:%02d\n",
                    root->key.data.timeValue.hour, root->key.data.timeValue.minute,
                    root->key.data.timeValue.second);
            break;
        }

        // Ricorsivamente salva i figli
        saveTreeToFile(root->left, file);
        saveTreeToFile(root->right, file);
    }
}

/* Deserializzazione */
void loadTreeFromFile(NodeLink *root, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Nessun file di salvataggio trovato.\n");
        return;
    }

    int id;
    char typeStr[20];
    char valueStr[100];

    while (fscanf(file, "%d,%19[^,],%99[^\n]\n", &id, typeStr, valueStr) == 3)
    {
        Value key;
        if (strcmp(typeStr, "TYPE_INT") == 0)
        {
            key.type = TYPE_INT;
            key.data.intValue = atoi(valueStr);
        }
        else if (strcmp(typeStr, "TYPE_STRING") == 0)
        {
            key.type = TYPE_STRING;
            key.data.stringValue = v_strdup(valueStr);
        }
        else if (strcmp(typeStr, "TYPE_BOOL") == 0)
        {
            key.type = TYPE_BOOL;
            key.data.boolValue = (strcmp(valueStr, "true") == 0);
        }
        else if (strcmp(typeStr, "TYPE_FLOAT") == 0)
        {
            key.type = TYPE_FLOAT;
            key.data.floatValue = atof(valueStr);
        }
        else if (strcmp(typeStr, "TYPE_DOUBLE") == 0)
        {
            key.type = TYPE_DOUBLE;
            key.data.doubleValue = atof(valueStr);
        }
        else if (strcmp(typeStr, "TYPE_DATE") == 0)
        {
            key.type = TYPE_DATE;
            sscanf(valueStr, "%d-%d-%d", &key.data.dateValue.year, &key.data.dateValue.month, &key.data.dateValue.day);
        }
        else if (strcmp(typeStr, "TYPE_TIME") == 0)
        {
            key.type = TYPE_TIME;
            sscanf(valueStr, "%d:%d:%d", &key.data.timeValue.hour, &key.data.timeValue.minute, &key.data.timeValue.second);
        }

        *root = insertNode(*root, key);

        /* Liberiamo la stringa originale dopo l'inserimento */
        if (key.type == TYPE_STRING)
        {
            free(key.data.stringValue);
        }
    }

    fclose(file);
}