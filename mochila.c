#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// --- Estruturas de Dados ---
typedef struct {
    int id;
    double peso;
    double valor;
    double razao; // valor / peso
} Item;

typedef struct {
    double max_w;
    double valor_total;
    double peso_total;
} Mochila;

// --- Declarações de Funções ---
void criarItens(Item* lista, int n, int w);
void imprimirItens(Item* lista, int n);
void heap_sort(Item** array, int size);
void guloso_alg(Item** itens, int n, Mochila* m);
void guloso(Item *itens, int n, double w);
void forca_bruta_recursiva(Item* itens, int n, double w, double valAtual, double pesoAtual, int i, double* maxVal, long long comb, long long* bestComb);
void forca_bruta(Item *itens, int n, double w);

// --- Programa Principal ---
int main() {
    int n; // Número de itens
    int w; // Peso máximo da mochila
    int k; // Seletor de algoritmo
    int s; // Seleciona entrada de itens

    printf("Insira o número de itens: ");
    scanf("%d", &n);

    printf("Insira o peso máximo da mochila: ");
    scanf("%d", &w);

    printf("Selecione o algorítmo: \n");
    printf("1. Força Bruta \n");
    printf("2. Guloso\n");
    scanf("%d", &k);

    Item itens[n];
    srand(10);

    printf("Adicionar itens ou gerar aleatórios?\n");
    printf("1. Adicionar itens manualmente\n");
    printf("2. Gerar itens aleatórios\n");
    scanf("%d", &s);

    if(s == 2){
        criarItens(itens, n, w);
        printf("\n--- Itens Gerados ---\n");
        imprimirItens(itens, n);
    }

    if(k == 1){
        forca_bruta(itens, n, w);
    }else if(k == 2){
        guloso(itens, n, w); 
    }
}

void forca_bruta(Item *itens, int n, double w){
        if (n >= 64) {
        printf("Erro: O número de itens deve ser menor que 64 para a solução de força bruta.\n");
        return;
        }
        // --- 2. Algoritmo de Força Bruta (CORRIGIDO) ---
        printf("\n--- Executando Solução Força Bruta ---\n");
        if (n > 30) {
            printf("(Aviso: n > 30, isso pode levar muito tempo...)\n");
        }

        double maxVal = 0.0;
        long long bestComb = 0LL;

        forca_bruta_recursiva(itens, n, (double)w, 0.0, 0.0, 0, &maxVal, 0LL, &bestComb);
        
        printf("Resultado Força Bruta -> Valor Máximo Encontrado: %.2f\n", maxVal);
        printf("Itens na melhor combinação:\n");

        double pesoFinal = 0.0;
        double valorFinal = 0.0;

        if (bestComb == 0 && maxVal == 0) {
            printf("  - Nenhum item foi selecionado.\n");
        } else {
            for (int i = 0; i < n; i++) {
                if ((bestComb >> i) & 1) {
                    printf("  - Item %d (Peso: %.2f, Valor: %.2f)\n", itens[i].id, itens[i].peso, itens[i].valor);
                    pesoFinal += itens[i].peso;
                    valorFinal += itens[i].valor;
                }
            }
        }
        printf("Peso Final da Força Bruta: %.2f\n", pesoFinal);
    }

void guloso(Item* itens, int n, double w){
    Item* item_pointers[n];
    for (int i = 0; i < n; i++) {
        item_pointers[i] = &itens[i];
    }
    // --- 1. Algoritmo Guloso ---
    printf("\n--- Executando Solução Gulosa ---\n");
    Mochila mochilaGuloso = { .max_w = w, .valor_total = 0, .peso_total = 0 };
    guloso_alg(item_pointers, n, &mochilaGuloso);
    printf("Resultado Guloso -> Valor Final: %.2f | Peso Final: %.2f\n", mochilaGuloso.valor_total, mochilaGuloso.peso_total);
}

///////////////// Algoritmo Guloso /////////////////
void guloso_alg(Item** itens, int n, Mochila* m) {

    heap_sort(itens, n);

    for (int i = 0; i < n; i++) {
        if (m->peso_total + itens[i]->peso <= m->max_w) {
            m->peso_total += itens[i]->peso;
            m->valor_total += itens[i]->valor;
            
            printf("Adicionado item %d (Peso: %.2f, Valor: %.2f, Razão: %.2f)\n", itens[i]->id, itens[i]->peso, itens[i]->valor, itens[i]->razao);
        }
    }
}

void forca_bruta_recursiva(Item* itens, int n, double w,
                           double valAtual, double pesoAtual, int i,
                           double* maxVal, long long comb, long long* bestComb)
{
    // --- Caso base: todos os itens foram processados ---
    if (i == n) {
        if (pesoAtual <= w && valAtual > *maxVal) {
            *maxVal = valAtual;
            *bestComb = comb;
        }
        return;
    }

    // --- 1. Ignora o item atual ---
    forca_bruta_recursiva(itens, n, w, valAtual, pesoAtual, i + 1,
                          maxVal, comb, bestComb);

    // --- 2. Inclui o item atual ---
    double novoPeso = pesoAtual + itens[i].peso;
    double novoValor = valAtual + itens[i].valor;

    // Só explora se ainda há chance de ser válido (Árvore com "poda", já que não explora toda a árvore)
    if (novoPeso <= w) {
        forca_bruta_recursiva(itens, n, w, novoValor, novoPeso, i + 1,
                              maxVal, comb | (1LL << i), bestComb);
    }
}

///////////////// Funções Utilitárias /////////////////
void criarItens(Item* lista, int n, int w) {
    for (int i = 0; i < n; i++) {
        double min_w = (double)w / (n + 5.0);
        double delta = ((double)rand() / RAND_MAX) * (2.5 * min_w);
        lista[i].id = i;
        lista[i].peso = min_w + delta;
        if (lista[i].peso < 0.1) lista[i].peso = 0.1; // Evitar peso zero
        lista[i].valor = lista[i].peso * (((double)rand() / RAND_MAX) * 4.0 + 0.5);
        lista[i].razao = lista[i].valor / lista[i].peso;
    }
}

void imprimirItens(Item* lista, int n) {
    int pesoTotal = 0;
    for (int i = 0; i < n; i++) {
        pesoTotal += lista[i].peso
        printf("Item %d -> Peso: %.2f | Valor: %.2f | Razão: %.2f\n", i, lista[i].peso, lista[i].valor, lista[i].razao);
    }
    printf("Peso total dos itens: %d\n", pesoTotal);
}


///////////////// Implementação do Heap Sort baseando se na razão /////////////////
void heapify(Item** array, int k, int size) {
    int l = 2 * k + 1;
    int r = 2 * k + 2;
    int min = k;

    if (l < size && array[l]->razao < array[min]->razao) {
        min = l;
    }
    if (r < size && array[r]->razao < array[min]->razao) {
        min = r;
    }
    if (min != k) {
        Item* aux = array[k];
        array[k] = array[min];
        array[min] = aux;
        heapify(array, min, size);
    }
}

void build_min_heap(Item** array, int size) {
    for (int i = size / 2 - 1; i >= 0; i--) {
        heapify(array, i, size);
    }
}

void heap_sort(Item** array, int size) {
    build_min_heap(array, size);
    for (int i = size - 1; i > 0; i--) {
        Item* aux = array[0];
        array[0] = array[i];
        array[i] = aux;
        heapify(array, 0, i);
    }
}