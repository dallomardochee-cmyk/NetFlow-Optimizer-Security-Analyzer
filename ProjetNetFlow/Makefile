
# Nom de l'exécutable final
TARGET = NetFlow_Sim.exe

# Compilateur
CC = gcc

# Options de compilation : 
# -Isrc : cherche les headers dans le dossier src
# -Wall : affiche tous les avertissements pour coder proprement
CFLAGS = -Isrc -Wall

# Liste de TOUS les fichiers sources
SRCS = src/main.c \
       src/graphe.c \
       src/dijkstra.c \
       src/securite.c \
       src/backtracking.c \
       src/utils.c \
       src/liste_chainee.c \
       src/simulation.c

# Transformation automatique des .c en .o (fichiers objets)
OBJS = $(SRCS:.c=.o)

# Règle par défaut : compile tout
all: $(TARGET)

# Liaison des fichiers objets pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Règle pour compiler chaque fichier .c en .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f src/*.o $(TARGET)
