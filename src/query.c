#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include <ctype.h>

RowLink *executeQuery(Table *table, const char *query, int *count)
{
    if (strncmp(query, "SELECT", 6) == 0)
    {
        printf("Eseguo SELECT...\n");

        // Controlliamo se c'è una condizione WHERE
        char columnName[50], conditionValue[50];
        int hasWhere = sscanf(query, "SELECT * FROM users WHERE %49[^=]=%49s", columnName, conditionValue);

        Value searchValue;
        if (hasWhere == 2)
        {
            if (isdigit(conditionValue[0]))
            {
                searchValue.type = TYPE_INT;
                searchValue.data.intValue = atoi(conditionValue);
            }
            else
            {
                searchValue.type = TYPE_STRING;
                searchValue.data.stringValue = conditionValue;
            }

            return searchRowsByColumn(table, columnName, searchValue, count);
        }
        else
        {
            // Se non c'è WHERE, restituisci tutte le righe
            RowLink *allRows = malloc(sizeof(RowLink) * 10);
            int allocatedSize = 10;
            *count = 0;

            Row *row = table->rows;
            while (row)
            {
                if (*count >= allocatedSize)
                {
                    allocatedSize *= 2;
                    allRows = realloc(allRows, sizeof(RowLink) * allocatedSize);
                }
                allRows[*count] = row;
                (*count)++;
                row = row->nextRow;
            }
            return allRows;
        }
    }
    else
    {
        printf("Query non supportata: %s\n", query);
        return NULL;
    }
}
