#include <stdio.h>
#include <stdlib.h>
#include "table.h"
#include "query.h"

int main()
{
    printf("Inizializzazione del database...\n");

    Table *users = createTable("users");
    addColumn(users, "id", TYPE_INT);
    addColumn(users, "name", TYPE_STRING);
    addColumn(users, "age", TYPE_INT);

    printf("Tabella 'users' creata con colonne: id, name, age\n");

    Value values1[] = {
        {.type = TYPE_INT, .data.intValue = 1},
        {.type = TYPE_STRING, .data.stringValue = "Giovanni"},
        {.type = TYPE_INT, .data.intValue = 25}};
    insertRow(users, values1);

    Value values2[] = {
        {.type = TYPE_INT, .data.intValue = 2},
        {.type = TYPE_STRING, .data.stringValue = "Paolo"},
        {.type = TYPE_INT, .data.intValue = 50}};
    insertRow(users, values2);

    Value values3[] = {
        {.type = TYPE_INT, .data.intValue = 3},
        {.type = TYPE_STRING, .data.stringValue = "Vinz"},
        {.type = TYPE_INT, .data.intValue = 26}};
    insertRow(users, values3);

    Value values4[] = {
        {.type = TYPE_INT, .data.intValue = 4},
        {.type = TYPE_STRING, .data.stringValue = "Max"},
        {.type = TYPE_INT, .data.intValue = 60}};
    insertRow(users, values4);

    Value values5[] = {
        {.type = TYPE_INT, .data.intValue = 5},
        {.type = TYPE_STRING, .data.stringValue = "Vio"},
        {.type = TYPE_INT, .data.intValue = 26}};
    insertRow(users, values5);

    printf("Righe inserite.\n");
    printTable(users);

    // Loop per leggere query utente
    char query[256];
    while (1)
    {
        printf("\n>> Inserisci una query (oppure 'exit' per uscire):\n");
        if (!fgets(query, sizeof(query), stdin))
            break;

        if (strncmp(query, "exit", 4) == 0)
            break;

        executeQuery(users, query);
    }

    freeTable(users);
    return 0;
}
