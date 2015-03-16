/*
Implemente um programa que, data uma topologia de rede e uma demanda de tráfego (em arquivos .txt), faça o roteamento da demanda de tráfego 
sobre a topologia e retorne um arquivo .txt com a capacidade necessária em cada enlace da topologia.
*/
//USANDO MONTAGEM DE ARQUIVO
// Quais são as entradas do dijsktra???

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

typedef int tipo_chave;

typedef struct nodo {
	tipo_chave chave;
	struct nodo *pai, *esq, *dir;
} nodo;

typedef struct BSTree {
	struct nodo *raiz; /* ponteiro para a raiz da árvore */
} BSTree;

void insira(BSTree *arvore,int k){//ponteiro para estrutura,chave a ser inserida
	nodo *no=NULL,*y=NULL,*yAnt;
	no = (nodo*)malloc(sizeof(nodo));//cria o novo nodo
	no->chave=k;
	no->pai=NULL;
	no->esq=NULL;
	no->dir=NULL;
	y=arvore->raiz;
	yAnt=NULL;
	while(y!=NULL){
		yAnt=y;
		if(k==y->chave)//não insere elementos iguais
			return;
		else if(k < y->chave)
			y=y->esq;
		else
			y=y->dir;
	}
	no->pai=yAnt;//NUNCA ESQUEÇA DISSO
	if (yAnt==NULL)//primeiro elemento
		arvore->raiz=no;
	else if(k < yAnt->chave)
		yAnt->esq=no;
	else
		yAnt->dir=no;
}

void imprima_chave(int k){
	printf("%d ",k);
}

void imprime(nodo *arvore){//raiz do ponteiro
	if(arvore!=NULL){
		imprime(arvore->esq);
		imprima_chave(arvore->chave);
		imprime(arvore->dir);
	}
}


int main(int tam_vet, char *parametros[]){
	FILE *topologia=NULL,*trafego=NULL;
	char linha[4],string1[4],string2[4]; 
	int num=0,v1=0,v2=0;
	
	BSTree *arvore;

	arvore=(BSTree*)malloc(sizeof(BSTree));
	arvore->raiz=NULL;
	
	topologia = fopen(parametros[1],"r");
	if(!topologia){
		printf("Erro na leitura do arquivo topologia \n");
		return 0;
	}
	
	fseek(topologia, 15, SEEK_SET);//POSICIONA NO COMEÇO dos vértices do grafo
	while((fscanf(topologia,"%d %d\n", &v1, &v2))!=EOF ){//le no formato esperado
		//insere na lista
		printf("%d %d ",v1, v2);
		printf("\n");
		
		//descobrir a quantidade de vértices
		//criar o grafo vetorzão
	}
	fclose(topologia);
	
	
	trafego = fopen(parametros[2],"r");
	if(!trafego){
		printf("Erro na leitura do arquivo trafego \n");
		return 0;
	}
	fclose(trafego);
	
	return 0;
}
/*	while((fgets(linha, sizeof(linha), topologia))!=NULL ){//fazendo leitura da linhas do arquivo topologia
	//poderia fechar o arquivo aqui?
		linha[strlen(linha-1)]='\0';
		if(isdigit(linha[0])){
			sscanf(linha,"%s",string1);//dividindo a string quando encontrar espaço
			//strcpy(caracter,linha[0]);
			//strncpy( caracter, linha, linha[0] );
			//caracter[strlen(caracter-1)]='\0';
			num = (int)atoi(string1); 
			printf("%d",num);
		}
		printf("%c",linha[0]);
		printf("%c",linha[2]);
		printf("%c",linha[3]);
		if(isdigit(linha[2])){
			printf("a");
			sscanf(linha,"%s",string2);//dividindo a string quando encontrar espaço
			num = (int)atoi(string2); 
			printf("%d",num);
			printf("\n");
		}
		
			//insere na lista
		//printf("%s", linha);
		//montar lista de ajacencia	
		memset(linha, 0, sizeof(linha));					//limpa o buffer para próxima leitura	
	}*/
