#include <stdio.h>
#include <stdlib.h>

typedef struct neighbour {
	short vertex;
	short flow;
	struct neighbour *next;	
} t_neighbour;

typedef struct {
	short V, A;
	t_neighbour **neighbours;
} t_graph;

typedef struct {
	short s;
	short t;
	short flow;
} t_traffic;

t_neighbour *NewNode(short w, t_neighbour *next, short flow);
void DigraphInsertA(t_graph *G, short v, short w, short cap);
void Inserir(t_graph  *G, short V, short W, short cap);
t_graph *new_graph(short n);
void new_link(t_graph *graph, short s, short t, short flow);
void best_path(t_graph *graph, short s, short t, short d, short n, int tt);
void Imprime(t_graph *G);

int main(int argc, char **argv) {

	/*if (!argv) {
		printf("Nenhum arquivo foi informado!\n");
		return 0;
	};*/
	
	FILE *fp,*trafego;
	if (!(fp = fopen(argv[1], "r"))) {
		printf("O primeiro arquivo não pode ser aberto!\n");
		return 0;
	};
	
	t_graph *graph;
	t_neighbour *a;
	t_traffic *traffic;
	short links, n, i=0, vs, vt, vetor[100];
	int tt,j=0;
	
	n = 0;
	tt = 1;
	links = 7;
	
	fseek(fp, 15, SEEK_SET);//POSICIONA NO COMEÇO dos vértices no arquivo
	while((fscanf(fp,"%hi %hi\n", &vs, &vt))!=EOF ) {
		if (vs > n) n = vs;
		if (vt > n) n = vt;
		vetor[i]=vs;//guarda as arestas v1 e v2 para inserir no grafo posteriormente
		vetor[i+1]=vt;
		
		i+=2;
		
	}
	fclose(fp);
	
	graph = new_graph(n);
	if(!graph)
		printf("O grafo nao foi montado!");
	

	//short q,b;

	for(j=0;n>0;j+=2){
		//scanf("%hi %hi", &q,&b);
		Inserir(graph,vetor[j],vetor[j+1],0);
		printf("mandando:  %hi %hi ",vetor[j], vetor[j+1]);
		//new_link(graph,vetor[j],vetor[j+1],0);
		printf("\n%d\n",j);
		n--;
	}
		
	trafego = fopen(argv[2],"r");
	if(!trafego){
		printf("Erro na leitura do arquivo trafego \n");
		return 0;
	}
	fclose(trafego);
	
	// e somar os tráfegos na variável tt (será usado como maior custo, no lugar de INFINITY)
	traffic = (t_traffic*)malloc(sizeof(t_traffic) * links);
	// encontrar o melhor caminho para cada par de tráfego
	for (i=0; i<links; i++)
		best_path(graph, traffic[i].s, traffic[i].t, traffic[i].flow, n, tt);
	// exibir o 'custo' final de cada aresta, que será o total de tráfego acumulado após rodar o algoritmo para todos os pares de tráfego
	
	for (i=0; i<n; i++){
		//printf("%p",graph->neighbours[i]);
		for (a=graph->neighbours[i]; a!=NULL; a=a->next){
			printf("\n%hi -> %hi: %hi\n", i, a->vertex, a->flow);
		}
	}
	Imprime(graph);
	return 0;
}

t_graph *new_graph(short n) {
	
	t_graph *graph;
	short i;
	
	graph = (t_graph*)malloc(sizeof(t_graph));
	graph->V = n;
	graph->neighbours = (t_neighbour**)malloc(sizeof(t_neighbour) * n);
	for (i=0; i<n; i++)
		graph->neighbours[i] = NULL;
	return graph;
}

void new_link(t_graph *graph, short s, short t, short flow) {
	
	t_neighbour *a, *i;
	
	a = (t_neighbour*)malloc(sizeof(t_neighbour));
	if(!a)
		return;
		
	i = (t_neighbour*)malloc(sizeof(t_neighbour));
	if(!i)
		return;
		
	a->vertex = t;
	a->flow = flow;
	
	if (graph->neighbours[s]) {
		for (i=graph->neighbours[s]; i->next!=NULL; i=i->next) {};
		i->next = a;
	}
	else
		graph->neighbours[s] = a;
	graph->A++;

}

t_neighbour *NewNode(short w, t_neighbour *next, short flow) { 
    t_neighbour *a = malloc(sizeof (t_neighbour));
    a->vertex = w;
    a->flow = flow;
    a->next = next;     
    return a;                         
}

void DigraphInsertA(t_graph *G, short v, short w, short cap) { 
    t_neighbour *a;
    for (a = G->neighbours[v]; a != NULL; a = a->next) 
        if (a->vertex == w) return;
    G->neighbours[v] = NewNode(w, G->neighbours[v], cap);

    //for (a = G->adj[w]; a != NULL; a = a->next) 
        //if (a->w == v) return;
    //G->adj[w] = NewNode(v, G->adj[w], cost);

    G->A++;
}

/*lê arestas do teclado*/
void Inserir(t_graph  *G, short V, short W, short cap) {

   //scanf("%hi %hi %hi", &V, &W, &cap);
    //if(V >= 0 && W >= 0 && V < G->V && W < G->V) 
    DigraphInsertA(G, V, W, cap);
   // else break;
    
}

void best_path(t_graph *graph, short s, short t, short d, short n, int tt) {
   
	short v0, w, mindist, fringe[n], dist[n], parent[n];
	t_neighbour *a;
	
	for (w = 0; w < graph->V; w++) {
		parent[w] = -1;
		dist[w] = 0;
	}
	parent[s] = s;
	dist[s] = 0;
	for (a = graph->neighbours[s]; a != NULL; a = a->next) { 
		dist[a->vertex] = a->flow;
		fringe[a->vertex] = s;
	}
	while (1) {
		mindist = tt;
		for (w = 0; w < graph->V; w++) 
			if (parent[w] == -1 && mindist > dist[w])
				mindist = dist[v0=w]; 
		if (mindist == tt) break;
		parent[v0] = fringe[v0];
		for (a = graph->neighbours[v0]; a != NULL; a = a->next) {
			if (parent[a->vertex] == -1 && dist[a->vertex] > dist[v0] + a->flow) { 
				dist[a->vertex] = dist[v0] + a->flow;
				a->flow += d;
				fringe[a->vertex] = v0;
				if (a->vertex == t) return;
			}
		}
	}
}

void Imprime(t_graph *G) {
    t_neighbour *a;
    int i;
    for(i = 0;i < G->V; i++) {
        for(a = G->neighbours[i];a != NULL; a = a->next) {
            printf("%hi -> %hi -- %hi\n", i, a->vertex, a->flow);
        }
    }
}

