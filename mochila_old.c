#include <stdio.h>
#include <stdlib.h> 
#include <time.h>


typedef struct{
    int id;
    double peso; // para usarmos MOD (%) precisamos de inteiros, isso nos leva a usar o peso em g e não em Kg
    int valor;
    double razao;
} item;

typedef struct{
    int maxW;
    item *itens_guardados;

} Mochila;

void criarItens(item *lista, int n, int w){
    for(int i = 0; i < n; i++){
        item aux;

        //Geração de valores aleatórios que não sejam todos perfeitamente finos ou maiores que W
        int min = w/n;
        double delta =  ((double) rand()/RAND_MAX) * (2 *min);
        printf("delta %d: %.2lf\n", i, delta); //debug
        
        aux.peso = min + delta;
        aux.valor = aux.peso * delta;
        aux.razao = aux.valor/aux.peso;
        aux.id = i;
        lista[i] = aux;
    }
}

void imprimirItens(item *lista, int n){
    int pesoTotal = 0;
    for(int i = 0; i < n; i++){
        pesoTotal += lista[i].peso;
        printf("%d Peso: %.2lf\n", i, lista[i].peso);
        printf("%d Valor: %d\n", i, lista[i].valor);
    }
    printf("Peso total dos itens: %d\n", pesoTotal);
}

void forcaBruta(item *lista, Mochila *mochila){
    
}


int main(){
    int n;
    int w;
    printf("Insira o número de itens: ");
    scanf("%d", &n);

    printf("Insira o peso máximo da mochila: ");
    scanf("%d", &w);
    
    item itens[n];
    srand(time(NULL)); //inicia o gerador de sementes a partir do tempo atual
    criarItens(itens, n, w);
    imprimirItens(itens, n);

    Mochila *mochila = (Mochila *) malloc(sizeof(mochila));
    mochila->maxW = w;
    

    return 0;
}

