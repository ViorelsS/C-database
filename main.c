#include <stdio.h>
#include "include/table.h"
#include "include/row.h"
#include "include/storage.h"

int main()
{
    printf("Inizializzazione del database...\n");

    // Creiamo una tabella di test
    Table *users = createTable("users");
    addColumn(users, "id", TYPE_INT);
    addColumn(users, "name", TYPE_STRING);
    addColumn(users, "age", TYPE_INT);

    printf("Tabella 'users' creata con colonne: id, name, age\n");

    // Inseriamo più righe nella tabella
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

    printf("Righe inserite:\n");
    printf("(1, Giovanni, 25)\n");
    printf("(2, Paolo, 50)\n");
    printf("(3, Vinz, 26)\n");
    printf("(4, Max, 60)\n");
    printf("(5, Vio, 26)\n");

    // Stampa i dati della tabella
    printTable(users);

    // Cleanup memoria
    freeTable(users);

    return 0;
}
