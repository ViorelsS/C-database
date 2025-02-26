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
    table->rows = NULL;
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

    // Initialize all values to avoid uninitialized usage
    for (int i = 0; i < colCount; i++)
    {
        newRow->values[i].type = TYPE_INT; // Default initialization
        newRow->values[i].data.intValue = 0;
    }

    /* Copia dei valori */
    col = table->columns; // Riportiamo il puntatore all'inizio della lista delle colonne
    for (int i = 0; i < colCount; i++)
    {
        newRow->values[i].type = values[i].type; // Copiamo il tipo

        if (values[i].type == TYPE_STRING)
        {
            if (values[i].data.stringValue != NULL)
            {
                newRow->values[i].data.stringValue = v_strdup(values[i].data.stringValue);
                if (!newRow->values[i].data.stringValue)
                {
                    printf("Errore: memoria insufficiente per allocare la stringa.\n");
                    exit(1);
                }
            }
            else
            {
                newRow->values[i].data.stringValue = NULL; // Evita accessi errati
            }
        }
        else
        {
            newRow->values[i] = values[i]; // Copia diretta per gli altri tipi
        }

        col = col->nextColumn; // Passa alla colonna successiva
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
    /* Inizializza colWidths a zero */
    memset(colWidths, 0, sizeof(colWidths));

    /* Cicla sulle colonne per determinare colWidths[i] */
    Column *c = table->columns;
    int i = 0;
    while (c)
    {
        /* Lunghezza minima = nome della colonna */
        int maxWidth = strlen(c->name);

        /* Cicla sulle righe per la colonna i */
        Row *row = table->rows;
        while (row)
        {
            if (row->values) /* se la riga ha valori */
            {
                /* A seconda del tipo, calcola la "string length" del contenuto */
                int valueLength = 0;
                switch (row->values[i].type)
                {
                case TYPE_STRING:
                    if (row->values[i].data.stringValue)
                        valueLength = strlen(row->values[i].data.stringValue);
                    break;
                case TYPE_INT:
                    /* Conta cifre in base 10, ad es. “25” => 2 caratteri */
                    valueLength = snprintf(NULL, 0, "%d", row->values[i].data.intValue);
                    break;
                case TYPE_FLOAT:
                    valueLength = snprintf(NULL, 0, "%.2f", row->values[i].data.floatValue);
                    break;
                case TYPE_DOUBLE:
                    valueLength = snprintf(NULL, 0, "%.2lf", row->values[i].data.doubleValue);
                    break;
                case TYPE_BOOL:
                    /* “true” o “false” => max 5 caratteri */
                    valueLength = row->values[i].data.boolValue ? 4 : 5;
                    break;
                case TYPE_DATE:
                    /* “YYYY-MM-DD” => 10 caratteri */
                    valueLength = 10;
                    break;
                case TYPE_TIME:
                    /* “HH:MM:SS” => 8 caratteri */
                    valueLength = 8;
                    break;
                }
                if (valueLength > maxWidth)
                    maxWidth = valueLength;
            }
            row = row->nextRow;
        }

        /* Aggiungiamo un po’ di padding, ad esempio +2 */
        colWidths[i] = maxWidth + 2;

        c = c->nextColumn;
        i++;
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
    Column *col = table->columns;
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
        if (currentRow->values == NULL)
        {
            currentRow = currentRow->nextRow;
            continue;
        }

        printf("|");
        for (i = 0; i < colCount; i++)
        {
            switch (currentRow->values[i].type)
            {
            case TYPE_INT:
                printf(" %-*d |", colWidths[i], currentRow->values[i].data.intValue);
                break;
            case TYPE_FLOAT:
                printf(" %-*.2f |", colWidths[i], currentRow->values[i].data.floatValue);
                break;
            case TYPE_DOUBLE:
                printf(" %-*.2lf |", colWidths[i], currentRow->values[i].data.doubleValue);
                break;
            case TYPE_BOOL:
                printf(" %-*s |", colWidths[i],
                       currentRow->values[i].data.boolValue ? "true" : "false");
                break;
            case TYPE_STRING:
                // già gestito
                printf(" %-*s |", colWidths[i], currentRow->values[i].data.stringValue);
                break;
            case TYPE_DATE:
                // es. "YYYY-MM-DD"
                printf(" %-*d-%d-%d |", colWidths[i],
                       currentRow->values[i].data.dateValue.year,
                       currentRow->values[i].data.dateValue.month,
                       currentRow->values[i].data.dateValue.day);
                break;
            case TYPE_TIME:
                // es. "HH:MM:SS"
                printf(" %-*d:%d:%d |", colWidths[i],
                       currentRow->values[i].data.timeValue.hour,
                       currentRow->values[i].data.timeValue.minute,
                       currentRow->values[i].data.timeValue.second);
                break;
            default:
                printf(" %-*s |", colWidths[i], "NULL");
                break;
            }
        }
        printf("\n");
        currentRow = currentRow->nextRow;
    }

    printSeparator(totalWidth, '=');
}

/* Funzione per liberare la memoria di una tabella */
void freeTable(Table *table)
{
    if (!table)
    {
        return;
    }

    /* Liberiamo il BST */
    if (table->root)
    {
        freeTree(table->root);
        table->root = NULL; // Impedisce accessi errati
    }

    /* Liberiamo le righe */
    Row *row = table->rows;
    while (row)
    {
        Row *tmp = row;
        if (tmp->values != NULL)
        {
            for (int i = 0; i < countColumns(table); i++)
            {
                if (tmp->values[i].type == TYPE_STRING && tmp->values[i].data.stringValue != NULL)
                {
                    free(tmp->values[i].data.stringValue);
                    tmp->values[i].data.stringValue = NULL;
                }
            }
            free(tmp->values);
            tmp->values = NULL;
        }
        row = row->nextRow;
        free(tmp);
    }
    table->rows = NULL;

    /* Liberiamo le colonne */
    Column *col = table->columns;
    while (col)
    {
        Column *tmp = col;
        col = col->nextColumn;
        free(tmp);
    }
    table->columns = NULL;

    free(table);
}
