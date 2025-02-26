# Nome del compilatore
CC = gcc

# Opzioni di compilazione
CFLAGS = -std=c99 -Wall -Wextra -g

# Nome degli eseguibili
TARGET = main
TEST_TARGET = test_main

# File sorgenti e oggetti
SRC = main.c bst.c
OBJ = $(SRC:.c=.o)
TEST_SRC = test_main.c bst.c
TEST_OBJ = $(TEST_SRC:.c=.o)

# Regola principale: crea l'eseguibile
all: $(TARGET)

# Compilazione dell'eseguibile principale
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compilazione dell'eseguibile per i test
$(TEST_TARGET): $(TEST_OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regola per compilare i singoli file .c in file .o
%.o: %.c bst.h
	$(CC) $(CFLAGS) -c $< -o $@

# Esegue il programma dopo la compilazione
run: all
	./$(TARGET)

# Esegue i test
test: $(TEST_TARGET)
	@echo "🔍 Eseguendo test con Valgrind..."
	@valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./$(TEST_TARGET)
	@echo "✅ Nessun memory leak trovato! Ora eseguo i test..."
	@./$(TEST_TARGET)

# Pulizia dei file generati
clean:
	rm -f $(OBJ) $(TEST_OBJ) $(TARGET) $(TEST_TARGET) database.txt database_test.txt database_test_extended.txt

# Comando per ricompilare tutto da zero
rebuild: clean all