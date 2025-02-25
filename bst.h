#ifndef BST_H
#define BST_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* Definizione del tipo di dati supportati */
typedef enum
{
    TYPE_INT,
    TYPE_STRING,
    TYPE_BOOL,

} DataType;

/* Definizione del valore generico */
typedef struct
{
    DataType type;
    union
    {
        int intValue;
        char *stringValue;
        bool boolValue;
    } data;
} Value;

/* Definizione del nodo del BST */
typedef struct node_s *NodeLink;
typedef struct node_s
{
    int id;    /* ID auto-incrementale */
    Value key; /* Chiave del nodo */
    NodeLink left;
    NodeLink right;
} Node;

/* Dichiarazione delle funzioni */
NodeLink createNode(Value key, int id);
NodeLink insertNode(NodeLink root, Value key);
NodeLink searchNodeById(NodeLink, int id);
NodeLink searchNodeByKey(NodeLink root, Value key);
NodeLink deleteNode(NodeLink root, Value key);
void inorderTraversal(NodeLink root);
void freeTree(NodeLink root);
void printValue(Value value);

/* Serializzazione e deserializzazione */
void saveTreeToFile(NodeLink root, FILE *file);
void loadTreeFromFile(NodeLink *root, const char *filename);

#endif