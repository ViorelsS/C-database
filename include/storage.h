#ifndef STORAGE_H
#define STORAGE_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

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

char *v_strdup(const char *s);

#endif