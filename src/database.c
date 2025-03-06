#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "database.h"

/* Creazione di un database vuoto */
Database *createDatabase()
{
    Database *db = (Database *)malloc(sizeof(Database));
    if (!db)
    {
        printf("Errore: memoria insufficiente per creare il database.\n");
        exit(1);
    }

    db->tables = NULL;
    return db;
}

/* Aggiungere una nuova tabella al database */
void addTable(Database *db, Table *table)
{
    TableNode *newNode = (TableNode *)malloc(sizeof(TableNode));
    if (!newNode)
    {
        printf("Errore: memoria insufficiente per aggiungere la tabella.\n");
        exit(1);
    }
    newNode->table = table;
    newNode->next = db->tables;
    db->tables = newNode;
}

/* Cerca una tabella per nome */
Table *getTable(Database *db, const char *name)
{
    TableNode *current = db->tables;
    while (current)
    {
        if (strcmp(current->table->name, name) == 0)
        {
            return current->table;
        }
        current = current->next;
    }
    return NULL;
}

/* Rimuovi una tabella dal database */
void dropTable(Database *db, const char *name)
{
    TableNode **indirect = &db->tables;

    while (*indirect)
    {
        if (strcmp((*indirect)->table->name, name) == 0)
        {
            TableNode *toDelete = *indirect;
            *indirect = (*indirect)->next;
            freeTable(toDelete->table);
            free(toDelete);
            return;
        }
    }
    indirect = &(*indirect)->next;
}

/* Elenca tutte le tabelle nel database */
void listTables(Database *db)
{
    TableNode *current = db->tables;
    printf("Tabelle nel database:\n");
    while (current)
    {
        printf("- %s\n", current->table->name);
        current = current->next;
    }
}

/* Libera tutt la memoria allocata dal database */
void freeDatabase(Database *db)
{
    TableNode *current = db->tables;
    while (current)
    {
        TableNode *toDelete = current;
        current = current->next;
        freeTable(toDelete->table);
        free(toDelete);
    }
    free(db);
}