# Nome del compilatore
CC = gcc

# Opzioni di compilazione
CFLAGS = -std=c99 -Wall -Wextra -g

# Nome dell'eseguibile
TARGET = bst_test

# File sorgenti e oggetti
SRC = main.c bst.c
OBJ = $(SRC:.c=.o)

# Regola principale: crea l'eseguibile
all: $(TARGET)

# Come costruire l'eseguibile dai file oggetto
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regola per compilare i singoli file .c in file .o
%.o: %.c bst.h
	$(CC) $(CFLAGS) -c $< -o $@

# Esegue il programma dopo la compilazione
run: all
	./$(TARGET)

# Pulizia dei file generati
clean:
	rm -f $(OBJ) $(TARGET)

# Comando per ricompilare tutto da zero
rebuild: clean all
