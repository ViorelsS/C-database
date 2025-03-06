CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Iinclude

TARGET = database
TEST_TARGET = test_db
SERVER_TARGET = server

SRC = main2.c src/storage.c src/table.c src/query.c
OBJ = src/storage.o src/table.o src/query.o 
TEST_SRC = tests/test_db.c src/storage.c
TEST_OBJ = $(TEST_SRC:.c=.o)
SERVER_SRC = server.c src/storage.c src/table.c src/query.c src/database.c
SERVER_OBJ = $(SERVER_SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ) main2.o
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

$(SERVER_TARGET): $(SERVER_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: all
	@echo "🔍 Eseguendo diagnosi con Valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(TARGET)
	@echo "✅ Nessun memory leak trovato!"
	@./$(TARGET)

test: $(TEST_TARGET)
	@echo "🔍 Eseguendo test con Valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(TEST_TARGET)
	@echo "✅ Nessun memory leak trovato! Ora eseguo i test..."
	@./$(TEST_TARGET)

run-server: $(SERVER_TARGET)
	@echo "🚀 Avvio del server C..."
	@./$(SERVER_TARGET)

clean:
	rm -f $(OBJ) $(TEST_OBJ) main2.o $(TARGET) $(TEST_TARGET) database.txt database_test.txt database_test_extended.txt
	
clean-server:
	rm -f $(OBJ) $(SERVER_OBJ) main2.o $(TARGET) $(SERVER_TARGET)

rebuild: clean all
