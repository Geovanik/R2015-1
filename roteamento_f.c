/* 
 * o programa deve receber dois arquivos, um contendo a topologia da rede e outro a matriz de tráfego
 * a primeira linha de cada arquivo deve ser um número, que indica quantas linhas (ligações ou tráfegos) existem
 * a topologia é descrita por 2-uplas (s t), que representam os vértices
 * o tráfego é descrito por 3-uplas (o d t), que representam a origem, o destino e o tráfego entre os dois
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define infinito 1000

typedef struct neighbour {
	short vertex, flow, length;
	struct neighbour *next;	
} t_neighbour;

typedef struct {
	short V, A;
	t_neighbour **neighbours;
} t_graph;

typedef struct {
	short s, t, flow;
} t_traffic;

t_neighbour *NewNode(short w, t_neighbour *next, short flow);
void DigraphInsertA(t_graph *G, short v, short w, short cap);
void Inserir(t_graph  *G, short V, short W, short cap);
void Imprime(t_graph *G);

t_graph *new_graph(short n);
void new_link(t_graph *graph, short s, short t, short flow);
void best_path(t_graph *graph, short s, short t, short d);
void distancias(t_graph *G, short s); 
void dfsR(t_graph *G, short v);
void DijkstraIngenuo(t_graph *G, short s, short destino);
int * encontra_caminho(t_graph *G, short s, short destino);


int main(int argc, char **argv) {
	
	short _n;
	t_graph *graph;
	t_traffic *traffic;
	int *caminhos=NULL,v=0;
	
	short i, vs, vt, tr;// prevo;
	
	FILE *fp;
	if (!(fp = fopen(argv[1], "r"))) {
		printf("O primeiro arquivo não pode ser aberto!\n");
		return 0;
	};
	
	fscanf(fp,"%hi", &_n); // lê N, que é a primeira linha 
	printf("numero de vertices = %hi\n", _n);	
	
	graph = new_graph(_n);//cria o grafo
	if(!graph) printf("O grafo nao foi montado!");
	
	printf("n = %hi\n", _n);
	for (i = 0; i < _n; i++) {//inicializando o grafo
		fscanf(fp,"%hi %hi\n", &vs, &vt);
		new_link(graph, vs-1, vt-1, 1);
	}
	fclose(fp);
	
	
	fp = fopen(argv[2],"r");
	if(!fp){
		printf("Erro na leitura do arquivo de tráfego \n");
		return 0;
	}

	fscanf(fp,"%hi", &tr); // lê a primeira linha, que dá a quantidade de linhas da matriz
	printf("linhas da matriz de trafego = %hi\n", tr);	
	traffic = (t_traffic*)malloc(sizeof(t_traffic) * tr);
	
	for (i = 0; i < tr; i++) {
	    fscanf(fp,"%hi %hi %hi\\n", &traffic[i].s, &traffic[i].t, &traffic[i].flow); //lendo a matriz de trafego
	    printf("mandando %hi %hi\n", traffic[i].s-1,traffic[i].t-1);
	    //DijkstraIngenuo(graph, traffic[i].s-1,traffic[i].t-1);
		caminhos=encontra_caminho(graph, traffic[i].s-1,traffic[i].t-1);
		printf("\n vetor: ");
	 for(v=0; v<6; v++) {
		printf("%hi \n",caminhos[v]); 
	 }
		//printf("%hi\n", traffic[i].s);
		//traffic[i].s--; traffic[i].t--;
		//break;
	}
	fclose(fp);
	
	
	
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

void new_link(t_graph *graph, short s, short t, short f) {
	
	t_neighbour *a, *i;
	
	a = (t_neighbour*)malloc(sizeof(t_neighbour));
	if(!a) return;

	i = (t_neighbour*)malloc(sizeof(t_neighbour));
	if(!i) return;
		
	a->vertex = t;
	a->flow = f;
	a->length = 1;
	a->next = NULL;
	
	if (graph->neighbours[s]) {
		for (i = graph->neighbours[s]; i->next!=NULL; i=i->next) {};
		i->next = a;
	}
	else
		graph->neighbours[s] = a;
	graph->A++;

}

void Imprime(t_graph *G) {
    t_neighbour *a;
    int i;
    for(i = 0;i < G->V; i++) {
        for(a = G->neighbours[i];a != NULL; a = a->next) {
            printf("%hi -> %hi -- %hi\n", i+1, a->vertex+1, a->flow);
        }
    }
}

void DijkstraIngenuo(t_graph *G, short s, short destino) {//encontrando o caminho mínimo e guardando em um vetor
    short v, w, v0, w0,i=0; 
    int controle=0;
    t_neighbour *a;
    short *parent = (short *) malloc(G->V*sizeof(short));
    double *dist = (double *) malloc(G->V*sizeof(double));
    int *vet_caminhos = (int *) malloc(((G->V)+2)*sizeof(int));
    
  
	for(i=0;i<((G->V)+2);i++){
		vet_caminhos[i]=-1;
		printf("%hi",vet_caminhos[i]);
    }
    for (w = 0; w < G->V; w++) //roda para quantidade de vértices no grafo
        parent[w] = -1, dist[w] =infinito;
    parent[s] = s; 
    dist[s] = 0.0; 
	

    while (1) {
        double mindist = infinito;
        for (v = 0; v < G->V; v++){ 
			if(v==0 && controle==0)
				vet_caminhos[v]=s;
            if (parent[v] != -1){ 
                for (a = G->neighbours[v]; a != NULL; a = a->next){
                    if (parent[a->vertex] == -1 && mindist > (dist[v] + a->flow)){
                        mindist = dist[v] + a->flow;
                        v0 = v, w0 = a->vertex;
                        if(a->vertex==destino && controle==0){
							vet_caminhos[v+1]=destino;
							printf("aqui  %hi ",vet_caminhos[v+1]);
							controle=1;
							printf("encontrei o destino na lista de adjacencia ");
						}
                    }
				}
				if(controle==0){
					vet_caminhos[v]=v;
					printf("NAO DEVO ENTRAR AQUI ");
				}	
			}
		}
        if (mindist == infinito) break; 
        /* A */
        parent[w0] = v0;
        dist[w0] = mindist;
    }
	printf("vertice analizado %hi ",s);
    for(v=0; v<4; v++) {
		printf("dist %.2lf = ", dist[v]);
	}
	printf("\n vetor: ");
	 for(v=0; v<6; v++) {
		printf("%hi \n",vet_caminhos[v]); 
	 }
}

int * encontra_caminho(t_graph *G, short s, short destino){
	t_neighbour *a;
	int i=0,v=0;
	int *vet_caminhos = (int *) malloc(((G->V)+2)*sizeof(int));
    
	for(i=0;i<((G->V)+2);i++){
		vet_caminhos[i]=-1;
	}
	vet_caminhos[0]=s;
	for(v = 0; v < G->V; v++){ 
		
		for (a = G->neighbours[v]; a != NULL; a = a->next){
			if(a->vertex==destino)
				vet_caminhos[v+1]=destino;
				printf("encontrei %hi \n",vet_caminhos[v+1]);
				return vet_caminhos;
		}
		vet_caminhos[v+1]=v+1;	
	}
	return vet_caminhos;
}
/*
 * void best_path(t_graph *graph, short s, short t, short d) {
   
	short v0, w, mindist, fringe[_n], dist[_n], parent[_n];
	t_neighbour *a;
	
	for (w = 0; w < graph->V; w++) {
		parent[w] = -1;
		dist[w] = _infinity;
	}
	parent[s] = s;
	dist[s] = 0;
	for (a = graph->neighbours[s]; a != NULL; a = a->next) { 
		dist[a->vertex] = a->length;
		fringe[a->vertex] = s;
	}
	while (1) {
		mindist = _infinity;
		for (w = 0; w < graph->V; w++) 
			if (parent[w] == -1 && mindist > dist[w])
				mindist = dist[v0=w]; 
		if (mindist == _infinity) break;
		parent[v0] = fringe[v0];
		for (a = graph->neighbours[v0]; a != NULL; a = a->next) {
			if (parent[a->vertex] == -1 && dist[a->vertex] > dist[v0] + a->length) { 
				dist[a->vertex] = dist[v0] + a->length;
				a->flow += d;
				fringe[a->vertex] = v0;
				if (a->vertex == t) return;
			}
		}
	}
}*/
