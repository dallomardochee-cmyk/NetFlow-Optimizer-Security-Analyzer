#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "graphe.h"

typedef struct { int id; float dist; } ElementHeap;
typedef struct { ElementHeap* data; int size; int capacity; } MinHeap;

void lancer_dijkstra(Graphe* g, int src, int dest);
void bellman_ford(Graphe* g, int src);

#endif