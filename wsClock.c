//Everton da Silva Coelho  101937
//Gabriel Borin Takahashi 101953
//Pedro Manhez Naresi 105615

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PAGINAS 500/10
#define MAX_RAM 250/10

#define TEMPO 10000/10
#define TAU 200/10
#define T_MEDICAO TAU*4/10

typedef struct Item{
    int id_page;
    int hit;
    long time;
}TItem;

typedef struct Celula TApontador;

typedef struct Celula{
    TItem item;
    TApontador *proximo;
    TApontador *anterior;
}TCelula;

typedef struct Lista{
    int tamanho;
    TApontador *frente;
}TLista;

void TLista_Inicia(TLista *lista){
    lista->tamanho=0;
    TCelula *cabeca = (TCelula*) malloc(sizeof(TCelula));
    lista->frente=cabeca;
    cabeca->proximo=lista->frente;
    cabeca->anterior=lista->frente;
}

int TLista_Tamanho(TLista *pLista){
    return pLista->tamanho;
}

int TLista_EhVazia(TLista *pLista){
    if(pLista->tamanho == 0)
        return 1;
    else
        return 0;
}

TApontador* TLista_Busca(TLista *lista, int x){
    TApontador *aux = lista->frente->proximo;
    while(aux!=lista->frente){
        if(aux->item.id_page == x){
            return aux;
        }
        aux=aux->proximo;
    }
    return NULL;
}

int TLista_Insere(TLista *lista, TApontador *p, TItem x){
    if(TLista_Tamanho(lista)==MAX_RAM){
        return 0;
    }
    else{
        TCelula *novo = (TCelula*)malloc(sizeof(TCelula));
    	novo->item=x;
        novo->proximo=p;
        novo->anterior=p->anterior;
        p->anterior->proximo=novo;
        p->anterior=novo;
        lista->tamanho++;
        return 1;
    }
}
/*
int TLista_Retira(TLista *lista, int n, TItem *x){
    if(TLista_EhVazia(lista)){
        printf("A lista nao possui elementos\n");
        return 0;
    }
    else{
        int i;
        TApontador *p;  //e a posicao de insercao é passada como referencia
        p=lista->frente->proximo;  //sempre insere na primeira posicao, dps da cabeca
        for(i=0; i<n; i++){
            p=p->proximo;
        }
        p=p->anterior;
        p->anterior->proximo=p->proximo;
        p->proximo->anterior=p->anterior;
        *x=p->item;
        free(p);
        lista->tamanho--;
        return 1;
    }
}*/

int TLista_Retira(TLista *lista, TApontador *p, TItem *x){
    if(TLista_EhVazia(lista)){
        printf("A lista nao possui elementos\n");
        return 0;
    }
    else{
        p=p->anterior;
        p->anterior->proximo=p->proximo;
        p->proximo->anterior=p->anterior;
        *x=p->item;
        free(p);
        lista->tamanho--;
        return 1;
    }
}

void printaLista(TLista *lista){
	TApontador *aux = lista->frente->proximo;
	while(aux!=lista->frente){
		printf("id_page: %d\t", aux->item.id_page);
        printf("page_hit: %d\t", aux->item.hit);
        printf("time_acess: %ld\n", aux->item.time);
		aux=aux->proximo;
	}
}

void Rand(TItem *x, int clock){
    x->id_page=rand() % MAX_PAGINAS;
    x->hit=1;
    x->time=clock;
}

TApontador* subsPage(TLista *lista, long clk){ //PROBLEMA TA AQUI RAAPAZZ
    TApontador *aux = lista->frente->proximo;
    TApontador *max_time;// = lista->frente->proximo; //segmentation
    long t=0;
    while(aux!=lista->frente){
        if(aux->item.hit==0){
        	if((aux->item.time - clk) > TAU){
	            return aux;
	        }
	        else if(aux->item.time > t){
	        	t = aux->item.time;
	        	max_time=aux;
	        }
	    }
        else{
            aux->item.hit=0;
        }
        aux=aux->proximo;
    }
    return max_time;
}

void zeraTudoOsHit(TLista *lista){
	TApontador *aux = lista->frente->proximo;
    while(aux!=lista->frente){
        aux->item.hit=0;
        aux=aux->proximo;
    }
}

void wsClock(TLista *lista){
    int i, count_hit=0, page_miss=0;
    long clock=0;
    TItem x;
    TApontador *posicao=lista->frente->proximo;
    TApontador *aux;
    for(i=0; i<TEMPO; i++){
        Rand(&x, clock);
        aux=TLista_Busca(lista, x.id_page);
        if(aux==NULL){ //nao acha na lista
            if(TLista_Tamanho(lista)<MAX_RAM){ //espaco livre
                TLista_Insere(lista, posicao, x);
                printf("INSERIU: %d \t\tclock: %ld\n", x.id_page, clock);
            }
            else{ //nao tem espaco livre
                TApontador *page_out;
                TItem xx;
                page_out=subsPage(lista, clock);
                if(page_out==NULL){

                }
                else{
                    if(TLista_Retira(lista, page_out, &xx)){
                    	if(TLista_Insere(lista, posicao, x)){
                    		printf("RETIROU: %d E INSERIU: %d\n", xx.id_page, x.id_page);
                    	}
                    	else printf("ERRO1\n");
                    }
                    else printf("ERRO2\n");
                }
            }
            page_miss++;
        }
        else{ //se achar
            aux->item.hit=1;
            aux->item.time=clock;
            count_hit++;
        }
        clock++;
        if(clock>=T_MEDICAO){
        	zeraTudoOsHit(lista);
        	clock=0;
        	printf("\t\t\tZEROU CLOCK\n");
        }
    }
    printf("count hit: %d\n", count_hit);
    printf("page miss: %d\n", page_miss);
}

int main(){    
    TLista lista;
    TLista_Inicia(&lista);
    wsClock(&lista);
    /*
    TItem aux;  //a insercao na lista eh atraves de um TItem
    TApontador *posicao = lista.frente->proximo;  //sempre insere na primeira posicao, dps da cabeca
    int i;
    for(i=1;i<=5;i++){
        aux.id_page=i*1000;
        aux.hit=i*10;
        aux.time=i*25.5;
        TLista_Insere(&lista, posicao, aux);
        printaLista(&lista);
        printf("\n");
    }
    TItem x;  
    TLista_Retira(&lista, 3, &x);
    printf("%d\n", TLista_Busca(&lista, 3000));
    printf("%d\n", TLista_Busca(&lista, 2000));
    */
    printaLista(&lista);
    return 0;
}