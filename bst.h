#ifndef BST_H
#define BST_H

/* Definizione del nodo del BST */
typedef struct node_s *NodeLink;
typedef struct node_s
{
    int key;
    NodeLink left;
    NodeLink right;
} Node;

/* Dichiarazione delle funzioni */
NodeLink createNode(int key);
NodeLink insertNode(NodeLink root, int key);
NodeLink searchNode(NodeLink root, int key);
NodeLink deleteNode(NodeLink root, int key);
void inorderTraversal(NodeLink root);
void freeTree(NodeLink root);

#endif