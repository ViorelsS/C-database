#include <stdio.h>  /* Funzioni di I/O */
#include <stdlib.h> /* Funzioni di gestione della memoria */
#include <string.h>
#include <unistd.h>    /* Fornisce funzioni come close() per gestire i file descriptor */
#include <arpa/inet.h> /* Contiene le funzioni per i socket e la comunicazione di rete */
#include "table.h"
#include "query.h"
#include "database.h"

#define PORT 8080        /* Il server TCP accetterà connessioni su questa porta */
#define BUFFER_SIZE 1024 /* Buffer per ricevere dati dal client */

void handle_client(int client_socket, Table *table)
{
    char buffer[BUFFER_SIZE];

    while (1)
    {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes_received = recv(client_socket, buffer, BUFFER_SIZE - 1, 0);
        if (bytes_received <= 0)
        {
            break;
        }

        buffer[bytes_received] = '\0';

        if (strncmp(buffer, "exit", 4) == 0)
        {
            break;
        }

        printf("Query ricevuta: %s\n", buffer);

        // Creiamo una risposta basata sui dati effettivi
        char response[BUFFER_SIZE] = "Risultati:\n";

        // Ottenere i risultati della query
        int count = 0;
        RowLink *results = executeQuery(table, buffer, &count);

        if (count > 0)
        {
            for (int i = 0; i < count; i++)
            {
                char row[100];

                // Costruiamo la stringa della riga basata sui valori effettivi
                snprintf(row, sizeof(row), "%d | %s | %d\n",
                         results[i]->values[0].data.intValue,
                         results[i]->values[1].data.stringValue,
                         results[i]->values[2].data.intValue);

                strcat(response, row);
            }
        }
        else
        {
            strcat(response, "Nessun risultato trovato.\n");
        }

        // Invia i dati al client
        send(client_socket, response, strlen(response), 0);

        // Libera la memoria usata per i risultati
        free(results);
    }

    close(client_socket);
}

int main()
{
    printf("Avvio del server database...\n");

    /* Creazione del database */
    Database *db = createDatabase();

    /* Creazione della tabella di test */
    Table *users = createTable("users");
    addColumn(users, "id", TYPE_INT);
    addColumn(users, "name", TYPE_STRING);
    addColumn(users, "age", TYPE_INT);
    addTable(db, users);

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

    /* Creazione del socket */
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        perror("Errore nella creazione del socket");
        exit(1);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    /* Associamo il socket alla porta */
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Errore nel binding");
        exit(1);
    }

    /* Mettiamo il server in ascolto */
    if (listen(server_socket, 5) < 0)
    {
        perror("Errore nella listen");
        exit(1);
    }

    printf("Server in ascolto sulla porta %d....\n", PORT);

    while (1)
    {
        struct sockaddr_in client_addr;
        socklen_t client_size = sizeof(client_addr);
        int client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_size);
        if (client_socket < 0)
        {
            perror("Errore nell'accept");
            continue;
        }

        printf("Nuova connessione accettata.\n");

        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        recv(client_socket, buffer, BUFFER_SIZE - 1, 0);

        if (strncmp(buffer, "LIST TABLES", 11) == 0)
        {
            char response[BUFFER_SIZE] = "Tabelle:\n";
            TableNode *current = db->tables;
            while (current)
            {
                strcat(response, current->table->name);
                strcat(response, "\n");
                current = current->next;
            }
            send(client_socket, response, strlen(response), 0);
        }
        else if (strncmp(buffer, "CREATE TABLE", 12) == 0)
        {
            char tableName[50];
            sscanf(buffer, "CREATE TABLE %s", tableName);
            Table *newTable = createTable(tableName);
            addTable(db, newTable);
            send(client_socket, "Tabella creata\n", 15, 0);
        }
        else if (strncmp(buffer, "FETCH TABLE", 11) == 0)
        {
            char tableName[50];
            sscanf(buffer, "FETCH TABLE %s", tableName);
            Table *table = getTable(db, tableName);
            if (table)
            {
                printTable(table);
                send(client_socket, "Tabella stampata\n", 18, 0);
            }
            else
            {
                send(client_socket, "Tabella non trovata\n", 21, 0);
            }
        }
        else
        {
            handle_client(client_socket, users);
            send(client_socket, "Comando non riconosciuto\n", 26, 0);
        }

        freeDatabase(db);
        close(server_socket);
        return 0;
    }
}