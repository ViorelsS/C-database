#include <stdio.h>
#include <stdlib.h>
#include "include/table.h"
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

    // Inseriamo più righe nella tabella con strdup()
    Value values1[] = {
        {.type = TYPE_INT, .data.intValue = 1},
        {.type = TYPE_STRING, .data.stringValue = v_strdup("Giovanni")},
        {.type = TYPE_INT, .data.intValue = 25}};
    insertRow(users, values1);

    Value values2[] = {
        {.type = TYPE_INT, .data.intValue = 2},
        {.type = TYPE_STRING, .data.stringValue = v_strdup("Paolo")},
        {.type = TYPE_INT, .data.intValue = 50}};
    insertRow(users, values2);

    Value values3[] = {
        {.type = TYPE_INT, .data.intValue = 3},
        {.type = TYPE_STRING, .data.stringValue = v_strdup("Vinz")},
        {.type = TYPE_INT, .data.intValue = 26}};
    insertRow(users, values3);

    Value values4[] = {
        {.type = TYPE_INT, .data.intValue = 4},
        {.type = TYPE_STRING, .data.stringValue = v_strdup("Max")},
        {.type = TYPE_INT, .data.intValue = 60}};
    insertRow(users, values4);

    Value values5[] = {
        {.type = TYPE_INT, .data.intValue = 5},
        {.type = TYPE_STRING, .data.stringValue = v_strdup("Vio")},
        {.type = TYPE_INT, .data.intValue = 26}};
    insertRow(users, values5);

    printf("Righe inserite:\n");
    printf("(1, Giovanni, 25)\n");
    printf("(2, Paolo, 50)\n");
    printf("(3, Vinz, 26)\n");
    printf("(4, Max, 60)\n");
    printf("(5, Vio, 26)\n");

    free(values1[1].data.stringValue);
    free(values2[1].data.stringValue);
    free(values3[1].data.stringValue);
    free(values4[1].data.stringValue);
    free(values5[1].data.stringValue);

    // Stampa i dati della tabella
    printf("Stampo la tabella...\n");

    Row *row = users->rows;
    while (row)
    {
        for (int i = 0; i < countColumns(users); i++)
        {
            printf("Colonna %d, Tipo: %d\n", i, row->values[i].type);
        }
        row = row->nextRow;
    }

    printTable(users);

    printf("\n🔍 Cerco utenti con età 26...\n");

    Value searchValue = {.type = TYPE_INT, .data.intValue = 26};
    int foundCount = 0;
    RowLink *foundRows = searchRowsByColumn(users, "age", searchValue, &foundCount);

    if (foundCount > 0)
    {
        printf("Trovate %d righe:\n", foundCount);
        for (int i = 0; i < foundCount; i++)
        {
            printf("ID: %d, Nome: %s, Età: %d\n",
                   foundRows[i]->values[0].data.intValue,
                   foundRows[i]->values[1].data.stringValue,
                   foundRows[i]->values[2].data.intValue);
        }
    }
    else
    {
        printf("Nessun risultato trovato.\n");
    }

    free(foundRows);

    // Cleanup memoria
    freeTable(users);

    return 0;
}
