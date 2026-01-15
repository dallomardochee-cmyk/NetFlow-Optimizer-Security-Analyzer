#include "securite.h"

// ============================================================================
// PARCOURS BFS (Breadth-First Search)
// ============================================================================

/**
 * @brief Parcours en largeur depuis un nœud source
 * @complexity O(V + E)
 */
void parcours_bfs(const Graphe *graphe, int source)
{
  if (!graphe)
    return;

  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int t = max_id + 1;

  int *visite = (int *)calloc(t, sizeof(int));
  FileFIFO *file = creer_file_fifo();

  printf("Parcours BFS depuis le nœud %d : ", source);

  enfiler_fifo(file, source);
  visite[source] = 1;

  while (!file_fifo_est_vide(file))
  {
    int u = defiler_fifo(file);
    printf("%d ", u);

    // Trouver l'index du nœud u
    int idx = -1;
    for (int i = 0; i < graphe->nb_noeuds; i++)
    {
      if (graphe->noeuds[i].id == u)
      {
        idx = i;
        break;
      }
    }

    if (idx != -1)
    {
      Arete *a = graphe->noeuds[idx].aretes;
      while (a)
      {
        int v = a->destination;
        if (v < t && !visite[v])
        {
          visite[v] = 1;
          enfiler_fifo(file, v);
        }
        a = a->suivant;
      }
    }
  }

  printf("\n");
  free(visite);
  liberer_file_fifo(file);
}

/**
 * @brief Parcours en profondeur (version itérative avec pile)
 * @complexity O(V + E)
 */
void parcours_dfs(const Graphe *graphe, int source)
{
  if (!graphe)
    return;

  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int t = max_id + 1;

  int *visite = (int *)calloc(t, sizeof(int));
  Pile *pile = creer_pile();

  printf("Parcours DFS depuis le nœud %d : ", source);

  empiler(pile, source);

  while (!pile_est_vide(pile))
  {
    int u = depiler(pile);

    if (visite[u])
      continue;

    visite[u] = 1;
    printf("%d ", u);

    // Trouver l'index du nœud u
    int idx = -1;
    for (int i = 0; i < graphe->nb_noeuds; i++)
    {
      if (graphe->noeuds[i].id == u)
      {
        idx = i;
        break;
      }
    }

    if (idx != -1)
    {
      Arete *a = graphe->noeuds[idx].aretes;
      while (a)
      {
        int v = a->destination;
        if (v < t && !visite[v])
        {
          empiler(pile, v);
        }
        a = a->suivant;
      }
    }
  }

  printf("\n");
  free(visite);
  liberer_pile(pile);
}

/**
 * @brief Détection de cycle via BFS (Algorithme de Kahn pour tri topologique)
 * Si le tri topologique ne peut pas inclure tous les nœuds, il y a un cycle.
 * @complexity O(V + E)
 */
int detecter_cycle_bfs(const Graphe *graphe)
{
  if (!graphe)
    return 0;

  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int t = max_id + 1;

  // Calculer les degrés entrants
  int *degre_entrant = (int *)calloc(t, sizeof(int));
  int *existe = (int *)calloc(t, sizeof(int));

  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    existe[graphe->noeuds[i].id] = 1;
    Arete *a = graphe->noeuds[i].aretes;
    while (a)
    {
      if (a->destination < t)
      {
        degre_entrant[a->destination]++;
      }
      a = a->suivant;
    }
  }

  // Enfiler tous les nœuds avec degré entrant 0
  FileFIFO *file = creer_file_fifo();
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    int id = graphe->noeuds[i].id;
    if (degre_entrant[id] == 0)
    {
      enfiler_fifo(file, id);
    }
  }

  int noeuds_visites = 0;

  while (!file_fifo_est_vide(file))
  {
    int u = defiler_fifo(file);
    noeuds_visites++;

    // Trouver l'index du nœud u
    int idx = -1;
    for (int i = 0; i < graphe->nb_noeuds; i++)
    {
      if (graphe->noeuds[i].id == u)
      {
        idx = i;
        break;
      }
    }

    if (idx != -1)
    {
      Arete *a = graphe->noeuds[idx].aretes;
      while (a)
      {
        int v = a->destination;
        if (v < t)
        {
          degre_entrant[v]--;
          if (degre_entrant[v] == 0)
          {
            enfiler_fifo(file, v);
          }
        }
        a = a->suivant;
      }
    }
  }

  free(degre_entrant);
  free(existe);
  liberer_file_fifo(file);

  // S'il reste des nœuds non visités, il y a un cycle
  return (noeuds_visites < graphe->nb_noeuds);
}

// ============================================================================
// DÉTECTION DE CYCLE (DFS - 3 couleurs)
// ============================================================================

static int dfs_cycle(const Graphe *g, int u, int *couleurs, int t)
{
  couleurs[u] = 1; // Gris

  // Trouver index
  int idx = -1;
  for (int i = 0; i < g->nb_noeuds; i++)
    if (g->noeuds[i].id == u)
    {
      idx = i;
      break;
    }

  if (idx != -1)
  {
    Arete *a = g->noeuds[idx].aretes;
    while (a)
    {
      int v = a->destination;
      if (v < t)
      {
        if (couleurs[v] == 1)
          return 1; // Retour sur un gris = cycle
        if (couleurs[v] == 0)
        {
          if (dfs_cycle(g, v, couleurs, t))
            return 1;
        }
      }
      a = a->suivant;
    }
  }

  couleurs[u] = 2; // Noir
  return 0;
}

int detecter_cycle(const Graphe *graphe)
{
  if (!graphe)
    return 0;

  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int t = max_id + 1;

  int *couleurs = (int *)calloc(t, sizeof(int));

  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    int u = graphe->noeuds[i].id;
    if (couleurs[u] == 0)
    {
      if (dfs_cycle(graphe, u, couleurs, t))
      {
        free(couleurs);
        return 1;
      }
    }
  }

  free(couleurs);
  return 0;
}

// --- Points d'Articulation (Graphe non-orienté ou orienté traité comme
// non-orienté) --- Note: Pour un graphe orienté, les points d'articulation se
// définissent souvent sur la version sous-jacente non-orientée ou alors via
// connexité forte. Ici on applique l'algo classique Tarjan sur undirected
// structure pour trouver les "Noeuds Critiques de Connectivité"

static int temps = 0;

static void dfs_art(const Graphe *g, int u, int p, int *visited, int *disc,
                    int *low, int *is_ap, int t)
{
  int children = 0;
  visited[u] = 1;
  disc[u] = low[u] = ++temps;

  int idx = -1;
  for (int i = 0; i < g->nb_noeuds; i++)
    if (g->noeuds[i].id == u)
    {
      idx = i;
      break;
    }

  if (idx != -1)
  {
    Arete *a = g->noeuds[idx].aretes;
    while (a)
    {
      int v = a->destination;
      if (v == p)
      {
        a = a->suivant;
        continue;
      }

      if (v < t && visited[v])
      {
        if (disc[v] < low[u])
          low[u] = disc[v];
      }
      else if (v < t)
      {
        children++;
        dfs_art(g, v, u, visited, disc, low, is_ap, t);

        if (low[v] < low[u])
          low[u] = low[v];

        if (p != -1 && low[v] >= disc[u])
          is_ap[u] = 1;
      }
      a = a->suivant;
    }
  }

  if (p == -1 && children > 1)
    is_ap[u] = 1;
}

void afficher_points_articulation(const Graphe *graphe)
{
  if (!graphe)
    return;

  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int t = max_id + 1;

  int *visited = (int *)calloc(t, sizeof(int));
  int *disc = (int *)calloc(t, sizeof(int));
  int *low = (int *)calloc(t, sizeof(int));
  int *is_ap = (int *)calloc(t, sizeof(int));
  temps = 0;

  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    int u = graphe->noeuds[i].id;
    if (!visited[u])
      dfs_art(graphe, u, -1, visited, disc, low, is_ap, t);
  }

  printf("Points d'Articulation (Noeuds Critiques) : ");
  int count = 0;
  for (int i = 0; i < t; i++)
  {
    if (is_ap[i])
    {
      printf("%d ", i);
      count++;
    }
  }
  if (count == 0)
    printf("Aucun");
  printf("\n");

  free(visited);
  free(disc);
  free(low);
  free(is_ap);
}

// --- Composantes Fortement Connexes (Tarjan) ---

// Pile utilitaire pour Tarjan
typedef struct NodeStack
{
  int data;
  struct NodeStack *next;
} NodeStack;

static void push(NodeStack **top, int v)
{
  NodeStack *node = (NodeStack *)malloc(sizeof(NodeStack));
  node->data = v;
  node->next = *top;
  *top = node;
}

static int pop(NodeStack **top)
{
  if (!*top)
    return -1;
  NodeStack *tmp = *top;
  int v = tmp->data;
  *top = tmp->next;
  free(tmp);
  return v;
}

static int min_val(int a, int b) { return (a < b) ? a : b; }

static void scc_util(const Graphe *g, int u, int *disc, int *low,
                     NodeStack **st, int *stackMember, int t)
{
  static int time = 0;
  disc[u] = low[u] = ++time;
  push(st, u);
  stackMember[u] = 1;

  // Trouver l'index du noeud u dans g->noeuds
  int idx = -1;
  for (int i = 0; i < g->nb_noeuds; i++)
    if (g->noeuds[i].id == u)
    {
      idx = i;
      break;
    }

  if (idx != -1)
  {
    Arete *a = g->noeuds[idx].aretes;
    while (a)
    {
      int v = a->destination;
      if (v < t)
      {
        if (disc[v] == -1)
        {
          scc_util(g, v, disc, low, st, stackMember, t);
          low[u] = min_val(low[u], low[v]);
        }
        else if (stackMember[v])
        {
          low[u] = min_val(low[u], disc[v]);
        }
      }
      a = a->suivant;
    }
  }

  // Si u est la racine d'une SCC
  if (low[u] == disc[u])
  {
    printf("SCC : { ");
    while (1)
    {
      int v = pop(st);
      printf("%d ", v);
      stackMember[v] = 0;
      if (u == v)
        break;
    }
    printf("}\n");
  }
}

void afficher_scc_tarjan(const Graphe *graphe)
{
  if (!graphe)
    return;

  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int t = max_id + 1;

  int *disc = (int *)malloc(t * sizeof(int));
  int *low = (int *)malloc(t * sizeof(int));
  int *stackMember = (int *)calloc(t, sizeof(int));
  NodeStack *st = NULL;

  for (int i = 0; i < t; i++)
  {
    disc[i] = -1;
    low[i] = -1;
  }

  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    int id = graphe->noeuds[i].id;
    if (disc[id] == -1)
      scc_util(graphe, id, disc, low, &st, stackMember, t);
  }

  free(disc);
  free(low);
  free(stackMember);
  // Vider st si reste (normalement vide)
  while (st)
    pop(&st);
}

// --- Ponts (Bridges) ---

static void dfs_bridge(const Graphe *g, int u, int p, int *visited, int *disc,
                       int *low, int t)
{
  static int timer_b = 0;
  visited[u] = 1;
  disc[u] = low[u] = ++timer_b;

  int idx = -1;
  for (int i = 0; i < g->nb_noeuds; i++)
    if (g->noeuds[i].id == u)
    {
      idx = i;
      break;
    }

  if (idx != -1)
  {
    Arete *a = g->noeuds[idx].aretes;
    while (a)
    {
      int v = a->destination;
      if (v == p)
      {
        a = a->suivant;
        continue;
      }
      if (visited[v])
      {
        low[u] = min_val(low[u], disc[v]);
      }
      else
      {
        dfs_bridge(g, v, u, visited, disc, low, t);
        low[u] = min_val(low[u], low[v]);
        if (low[v] > disc[u])
        {
          printf("Pont détecté : %d -- %d\n", u, v);
        }
      }
      a = a->suivant;
    }
  }
}

void afficher_ponts(const Graphe *graphe)
{
  if (!graphe)
    return;

  int max_id = 0;
  for (int i = 0; i < graphe->nb_noeuds; i++)
    if (graphe->noeuds[i].id > max_id)
      max_id = graphe->noeuds[i].id;
  int t = max_id + 1;

  int *visited = (int *)calloc(t, sizeof(int));
  int *disc = (int *)calloc(t, sizeof(int));
  int *low = (int *)calloc(t, sizeof(int));

  printf("Ponts (Arêtes Critiques) : \n");
  for (int i = 0; i < graphe->nb_noeuds; i++)
  {
    int u = graphe->noeuds[i].id;
    if (!visited[u])
      dfs_bridge(graphe, u, -1, visited, disc, low, t);
  }
  printf("(Fin des ponts)\n");

  free(visited);
  free(disc);
  free(low);
}
