#ifndef TABLE_H
#define TABLE_H

#include "types.h"
#include "storage.h"

typedef struct column_s *ColumnLink;
typedef struct row_s *RowLink;

/* Definizione della colonna */
typedef struct column_s
{
    char name[50];         /* Nome della colonna*/
    DataType type;         /* Lista delle colonne */
    ColumnLink nextColumn; /* Prossima colonna */
} Column;

/* Definizione della struttura Row */
typedef struct row_s
{
    int id;          /* ID univoco */
    Value *values;   /* Array di valori della riga */
    RowLink nextRow; /* Puntatore alla prossima riga */
} Row;

/* Definizione della tabella */
typedef struct table_s
{
    char name[50];   /* Nome della tabella */
    Column *columns; /* Lista delle colonne */
    NodeLink root;   /* BST delle righe */
    Row *rows;       /* Lista delle righe */
} Table;

/* Funzioni per la gestione delle tabelle */
Table *createTable(const char *name);
void addColumn(Table *table, const char *colName, DataType type);
void insertRow(Table *table, Value *values);
void printTable(Table *table);
void freeTable(Table *table);

RowLink *searchRowsByColumn(Table *table, const char *columnName, Value value, int *count);

/* Funzione per contare le colonne */
int countColumns(Table *table);

#endif