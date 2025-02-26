#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

/* -----------------------------------------*/
/* --------------- SUPPORTO ----------------*/
/* -----------------------------------------*/

/* Funzione per contare le colonne */
int countColumns(Table *table)
{
    int count = 0;
    Column *col = table->columns;
    while (col)
    {
        count++;
        col = col->nextColumn;
    }
    return count;
}

/* Funzione per stampare un separatore */
void printSeparator(int width, char sep)
{
    printf("+");
    for (int i = 0; i < width - 2; i++)
    {
        printf("%c", sep);
    }
    printf("+\n");
}

/* Funzione per calcolare la larghezza totale della tabella */
int calculateTotalWidth(Table *table)
{
    int totalWidth = 2; // Due spazi per i bordi
    Column *col = table->columns;
    while (col)
    {
        totalWidth += strlen(col->name) + 3; // Nome colonna + padding
        col = col->nextColumn;
    }
    return totalWidth;
}

/* -----------------------------------------*/
/* ---------------- TABLE ------------------*/
/* -----------------------------------------*/

/* Funzione per la creazione nuova tabella */
Table *createTable(const char *name)
{
    Table *table = (Table *)malloc(sizeof(Table));
    if (!table)
    {
        printf("Errore: memoria insufficiente per creare la tabella.\n");
        exit(1);
    }

    strncpy(table->name, name, sizeof(table->name) - 1);
    table->name[sizeof(table->name) - 1] = '\0';
    table->columns = NULL;
    table->root = NULL;
    return table;
}

/* Funzione per aggiungere una colonna a una tabella */
void addColumn(Table *table, const char *colName, DataType type)
{
    Column *newCol = (Column *)malloc(sizeof(Column));
    if (!newCol)
    {
        printf("Errore: memoria insufficiente per creare la colonna.\n");
        exit(1);
    }
    strncpy(newCol->name, colName, sizeof(newCol->name) - 1);
    newCol->name[sizeof(newCol->name) - 1] = '\0';
    newCol->type = type;
    newCol->nextColumn = NULL;

    /* Se la lista è vuota, assegnamo direttamente */
    if (table->columns == NULL)
    {

        table->columns = newCol;
    }
    else
    {
        /* Aggiungiamo alla fine per preservare l'ordine */
        Column *current = table->columns;
        while (current->nextColumn != NULL)
        {
            current = current->nextColumn;
        }
        current->nextColumn = newCol;
    }
}

/* Funzione per inserire una riga nella tabella */
void insertRow(Table *table, Value *values)
{
    static int currentId = 1;

    /* Creazione della riga */
    Row *newRow = (Row *)malloc(sizeof(Row));
    if (!newRow)
    {
        printf("Errore: memoria insufficiente per creare la riga.\n");
        exit(1);
    }

    /* Impostiamo l'ID */
    newRow->id = currentId++;

    /* Conta il numero di colonne */
    int colCount = 0;
    Column *col = table->columns;
    while (col)
    {
        colCount++;
        col = col->nextColumn;
    }

    /* Allocazione dei valori */
    newRow->values = (Value *)malloc(sizeof(Value) * colCount);
    if (!newRow->values)
    {
        printf("Errore: memoria insufficiente per allocare i valori della riga.\n");
        free(newRow);
        exit(1);
    }

    /* Copia dei valori */
    for (int i = 0; i < colCount; i++)
    {
        /* Copia diretta */
        newRow->values[i] = values[i];
    }

    /* Inseriamo la riga nella lista delle righe */
    newRow->nextRow = table->rows;
    table->rows = newRow;

    /* Il primo valore è la chiave primaria */
    table->root = insertNode(table->root, values[0]);
}

/* Funzione per stampare il contenuto della tabella */
void printTable(Table *table)
{
    if (!table)
    {
        printf("Errore: la tabella è NULL.\n");
        return;
    }

    /* Contiamo il numero di colonne */
    int colCount = countColumns(table);
    if (colCount == 0)
    {
        printf("La tabella non ha colonne.\n");
        return;
    }

    /* Calcoliamo la larghezza massima per ogni colonna */
    int colWidths[colCount];
    Column *col = table->columns;
    int i = 0;

    while (col)
    {
        /* Inizialmente la lunghezza del nome della colonna */
        int maxWidth = strlen(col->name);
        Row *row = table->rows;

        while (row)
        {
            int valueLength = 0;
            if (row->values[i].type == TYPE_STRING)
            {
                valueLength = strlen(row->values[i].data.stringValue);
            }
            else
            {
                valueLength = snprintf(NULL, 0, "%d", row->values[i].data.intValue);
            }

            if (valueLength > maxWidth)
            {
                maxWidth = valueLength;
            }
            row = row->nextRow;
        }

        /* Aggiungiamo padding */
        colWidths[i++] = maxWidth + 2;
        col = col->nextColumn;
    }

    /* Calcolo della larghezza totale della tabella */
    int totalWidth = 1; // Il bordo iniziale "+"
    for (i = 0; i < colCount; i++)
        totalWidth += colWidths[i] + 3; // Spazio tra colonne

    /* Stampa il bordo superiore e il nome della tabella centrato */
    printSeparator(totalWidth, '=');

    int titlePadding = (totalWidth - 3 - strlen(table->name)) / 2;
    printf("|%*s\033[34m%s\033[0m%*s|\n", titlePadding, "", table->name, titlePadding + 1, "");

    printSeparator(totalWidth, '=');

    /* Stampa le intestazioni delle colonne */
    printf("|");
    col = table->columns;
    i = 0;
    while (col)
    {
        printf(" \033[33m%-*s\033[0m |", colWidths[i], col->name);
        col = col->nextColumn;
        i++;
    }
    printf("\n");

    printSeparator(totalWidth, '-');

    /* Stampa i dati della tabella */
    Row *currentRow = table->rows;
    while (currentRow)
    {
        printf("|");
        for (i = 0; i < colCount; i++)
        {
            if (currentRow->values[i].type == TYPE_STRING)
                printf(" \033[36m%-*s\033[0m |", colWidths[i], currentRow->values[i].data.stringValue);
            else
                printf(" \033[32m%-*d\033[0m |", colWidths[i], currentRow->values[i].data.intValue);
        }
        printf("\n");
        currentRow = currentRow->nextRow;
    }

    printSeparator(totalWidth, '=');
}

/* Funzione per liberare la memoria di una tabella */
void freeTable(Table *table)
{
    freeTree(table->root);
    Column *col = table->columns;
    while (col)
    {
        Column *tmp = col;
        col = col->nextColumn;
        free(tmp);
    }
    free(table);
}