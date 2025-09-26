#include <stdio.h>

typedef struct{
    double peso;
    int valor;
} item;

typedef struct{
    double maxW;
    //item *itens_guardados;

} Mochila;

int main(){
    int n, w; //n é o número de itens, w o peso máximo suportado pela mochila
    printf("Insira o número de itens: ");
    scanf("%d", &n);

    printf("Insira o peso máximo: ");
    scanf("%d", &w);
    
    item *itens;
    itens = (item *) calloc(n, sizeof(item));

    //Será que é realmente necessário guardar os ítens em um vetor simulando a mochila?
    //item *guardados;
    //guardados = (item *) calloc(w, sizeof(item));

    Mochila *mochila;
    mochila->maxW = w;

    return 0;
}

void forcaBruta(item *lista, Mochila *mochila){
    
}