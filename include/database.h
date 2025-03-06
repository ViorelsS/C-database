#ifndef DATABASE_H
#define DATABASE_H

#include "table.h"

typedef struct table_node_s *NextTable;

typedef struct table_node_s
{
    Table *table;
    NextTable next;
} TableNode;

typedef struct
{
    TableNode *tables;
} Database;

/* Funzioni di gestione del database */
Database *createDatabase();
void addTable(Database *db, Table *table);
Table *getTable(Database *db, const char *name);
void dropTable(Database *db, const char *name);
void listTables(Database *db);
void freeDatabase(Database *db);

#endif