#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"
#include <ctype.h>

void executeQuery(Table *table, const char *query)
{
    if (strncmp(query, "SELECT", 6) == 0)
    {
        // Estrarre la colonna e la condizione (se presente)
        char columnName[50], conditionColumn[50], conditionValue[50];
        int hasWhere = sscanf(query, "SELECT %49[^ ] FROM %*s WHERE %49[^=]=%49s", columnName, conditionColumn, conditionValue);

        if (hasWhere == 3)
        {
            printf("Eseguo SELECT su colonna '%s' con WHERE %s = %s...\n", columnName, conditionColumn, conditionValue);

            // Convertiamo il valore della condizione in un Value
            Value searchValue;
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

            // Cerchiamo i risultati
            int count = 0;
            RowLink *rows = searchRowsByColumn(table, conditionColumn, searchValue, &count);

            // Stampiamo i risultati
            if (count > 0)
            {
                for (int i = 0; i < count; i++)
                {
                    printf("ID: %d, Nome: %s, Età: %d\n",
                           rows[i]->values[0].data.intValue,
                           rows[i]->values[1].data.stringValue,
                           rows[i]->values[2].data.intValue);
                }
            }
            else
            {
                printf("Nessun risultato trovato.\n");
            }
            free(rows);
        }
        else
        {
            printf("Eseguo SELECT su tutte le righe della tabella...\n");
            printTable(table);
        }
    }
    else if (strncmp(query, "INSERT", 6) == 0)
    {
        int id, age;
        char name[50];

        if (sscanf(query, "INSERT INTO users (id, name, age) VALUES (%d, \"%49[^\"]\", %d);", &id, name, &age) == 3)
        {
            printf("Eseguo INSERT INTO users VALUES (%d, %s, %d)\n", id, name, age);
            Value values[] = {
                {.type = TYPE_INT, .data.intValue = id},
                {.type = TYPE_STRING, .data.stringValue = name},
                {.type = TYPE_INT, .data.intValue = age}};
            insertRow(table, values);
        }
        else
        {
            printf("Errore nella sintassi di INSERT.\n");
        }
    }
    else if (strncmp(query, "DELETE", 6) == 0)
    {
        int id;
        if (sscanf(query, "DELETE FROM users WHERE id=%d;", &id) == 1)
        {
            printf("Eseguo DELETE WHERE id=%d\n", id);
            Value key = {.type = TYPE_INT, .data.intValue = id};
            table->root = deleteNode(table->root, key);
        }
        else
        {
            printf("Errore nella sintassi di DELETE.\n");
        }
    }
    else
    {
        printf("Comando non riconosciuto: %s\n", query);
    }
}
