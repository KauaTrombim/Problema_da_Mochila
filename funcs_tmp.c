#include <stdio.h>

typedef struct item_s{
    double peso;
    double valor;
    double razao;
}item;

typedef struct mochila_s{
    //precisa de uma lista encadeada
    double w;
    double valor;
    double peso;

}mochila;

//////////////// guloso /////////////////

void guloso(item* itens[], int n, mochila* m){

    double peso = 0;
    double valor = 0;

    heap_sort(itens, n); // ordena o vetor de itens por valor/peso decrescente

    for(int i = 0; i < n; i++){

        if(peso + itens[i]->peso <= m->w){ 
            peso += itens[i]->peso;
            valor += itens[i]->valor;
            inserir_item(m, itens[i]); 
        }
    }

    m->peso = peso;
    m->valor = valor;
}

//////////////// forca bruta /////////////////

void forca_bruta(item** itens, int n, double w, double valAtual, double pesoAtual, int i, double* maxVal, long long comb, long long* bestComb) {
    if (pesoAtual > w) return; // não continua se passou do limite
    if (i == n) {
        if (valAtual > *maxVal) {
            *maxVal = valAtual;
            *bestComb = comb;
        }
        return;
    }

    // não pega o item i
    forca_bruta(itens, n, w, valAtual, pesoAtual, i + 1, maxVal, comb, bestComb);

    // pega o item i
    forca_bruta(itens, n, w, valAtual + itens[i]->valor, pesoAtual + itens[i]->peso, i + 1, maxVal, comb | (1LL << i), bestComb);
}

/*---------------heap pra item--------------------*/

void heapify(item** array, int k, int size){
    int l = 2*k + 1;
    int r = 2*k + 2;

    int max = k;

    if(l < size && array[l]->razao > array[max]->razao){
        max = l;
    }
    if(r < size && array[r]->razao > array[max]->razao){
        max = r;
    }
    if(max != k){
        item* aux = array[k];
        array[k] = array[max];
        array[max] = aux;

        heapify(array, max, size);
    }
    
}

void build_max_heap(item** array, int size){
    int k = size/2 - 1;

    for(int i = k; i >= 0; i--){
        heapify(array, i, size);
    }
}

void heap_sort(item** array, int size){

    build_max_heap(array, size);

    for(int i = 0; i < size; i++){
        item* aux = array[size - 1 - i];
        array[size - 1 - i] = array[0];
        array[0] = aux;
        heapify(array, 0, size - 1 - i);
    }
}

//////////////// Programação Dinâmica /////////////////

void prog_dinamica(item *itens[], ){


}