CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -Iinclude

TARGET = database
TEST_TARGET = test_db

SRC = main.c src/storage.c src/table.c src/row.c src/relation.c src/db.c
OBJ = src/storage.o src/table.o src/row.o src/relation.o src/db.o
TEST_SRC = tests/test_db.c src/storage.c
TEST_OBJ = $(TEST_SRC:.c=.o)

all: $(TARGET)

$(TARGET): $(OBJ) main.o
	$(CC) $(CFLAGS) -o $@ $^

$(TEST_TARGET): $(TEST_OBJ)
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

clean:
	rm -f $(OBJ) $(TEST_OBJ) main.o $(TARGET) $(TEST_TARGET) database.txt database_test.txt database_test_extended.txt

rebuild: clean all
