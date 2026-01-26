# Nom de l'exécutable final
TARGET = NetFlow_Sim.exe

# Compilateur
CC = gcc

# Options de compilation 
# -lm : lie la bibliothèque mathématique (pour sqrt, pow, etc.)
# -g  : ajoute les symboles de débogage (utile pour gdb ou valgrind)
CFLAGS = -Isrc -Wall -g
LDFLAGS = -lm

# Liste de TOUS les fichiers sources
SRCS = src/main.c \
       src/graphe.c \
       src/dijkstra.c \
       src/securite.c \
       src/backtracking.c \
       src/utils.c \
       src/liste_chainee.c \
       src/simulation.c

# Transformation automatique des .c en .o
OBJS = $(SRCS:.c=.o)

# Règle par défaut
all: $(TARGET)

# Liaison des fichiers objets
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Règle pour compiler chaque fichier .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer
clean:
	rm -f src/*.o $(TARGET)

# Empêche les conflits avec des fichiers nommés 'clean' ou 'all'
.PHONY: all clean