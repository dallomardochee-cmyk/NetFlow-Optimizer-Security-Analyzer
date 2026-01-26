CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
LDFLAGS = 

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
TEST_DIR = tests

# Lister les sources du programme principal (tout sauf le main pour les tests si besoin, mais ici on sépare)
# Pour éviter de lier main.o avec tests_unitaires.o (conflit de main)
SRCS_LIB = $(filter-out $(SRC_DIR)/main.c, $(wildcard $(SRC_DIR)/*.c))
OBJS_LIB = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS_LIB))

# Objet main
OBJ_MAIN = $(OBJ_DIR)/main.o

# Cible principale
TARGET = $(BIN_DIR)/netflow_optimizer

# Tests
TEST_SRC = $(TEST_DIR)/tests_unitaires.c
TEST_OBJ = $(OBJ_DIR)/tests_unitaires.o
TEST_TARGET = $(BIN_DIR)/tests_runner

.PHONY: all clean debug test

all: $(TARGET)

$(TARGET): $(OBJS_LIB) $(OBJ_MAIN)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Cible de test
test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(OBJS_LIB) $(TEST_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/tests_unitaires.o: $(TEST_SRC)
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

debug: CFLAGS += -DDEBUG
debug: all
