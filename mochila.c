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
void guloso(Item *itens, int n, double w);
void forca_bruta_recursiva(Item* itens, int n, double w, double valAtual, double pesoAtual, int i, double* maxVal, long long comb, long long* bestComb);
void forca_bruta(Item *itens, int n, double w);

// --- Programa Principal ---
int main() {
    int n; // Número de itens
    int w; // Peso máximo da mochila
    int a; // Seletor de algoritmo
    int s; // Seleciona entrada de itens

    printf("Insira o número de itens: ");
    scanf("%d", &n);

    printf("Insira o peso máximo da mochila: ");
    scanf("%d", &w);

    printf("Selecione o algorítmo: \n");
    printf("1. Força Bruta \n");
    printf("2. Guloso\n");
    printf("3. Programação Dinâmica\n");
    scanf("%d", &a);

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

    if(a == 1){
        forca_bruta(itens, n, w);
    }else if(a == 2){
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

    printf("Itens na melhor combinação:\n");

    double pesoFinal = 0.0;
    double valorFinal = 0.0;

    if (bestComb == 0 && maxVal == 0) {
        printf("  - Nenhum item foi selecionado.\n");
    } else {
    for (int i = 0; i < n; i++) {
        if ((bestComb >> i) & 1) {
            printf("Adicionado Item %d (Peso: %.2f, Valor: %.2f)\n", itens[i].id, itens[i].peso, itens[i].valor);
                pesoFinal += itens[i].peso;
                valorFinal += itens[i].valor;
            }
         }
    }
        printf("\nResultado Força Bruta -> Valor Final: %.2f | Peso Final: %.2f\n", valorFinal, pesoFinal);
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

    // --- Ignora o item atual ---
    forca_bruta_recursiva(itens, n, w, valAtual, pesoAtual, i + 1,
                          maxVal, comb, bestComb);

    // --- Inclui o item atual ---
    double novoPeso = pesoAtual + itens[i].peso;
    double novoValor = valAtual + itens[i].valor;

    // Só explora se ainda há chance de ser válido (Árvore com "poda", já que não explora toda a árvore)
    if (novoPeso <= w) {
        forca_bruta_recursiva(itens, n, w, novoValor, novoPeso, i + 1,
                              maxVal, comb | (1LL << i), bestComb);
    }
}

// --- Função de comparação para qsort ---
// --- Ordena os itens em ordem decrescente com base na sua razão (valor/peso) ---
int compareItens(const void *a, const void *b) {
    // Converte os ponteiros void* para ponteiros do tipo Item*
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;

    // Compara as razões para ordenação decrescente
    if (itemA->razao < itemB->razao) {
        return 1;
    } else if (itemA->razao > itemB->razao) {
        return -1;
    } else {
        return 0; // São iguais
    }
}

void guloso(Item* itens, int n, double w){
    qsort(itens, n, sizeof(Item), compareItens);
  
    Mochila mochilaGuloso = { .max_w = w, .valor_total = 0, .peso_total = 0 };

    printf("\n--- Executando Solução Gulosa ---\n");

    // Itera sobre os itens já ordenados e preenche a mochila
    for (int i = 0; i < n; i++) {
        if (mochilaGuloso.peso_total + itens[i].peso <= mochilaGuloso.max_w) {
            mochilaGuloso.peso_total += itens[i].peso;
            mochilaGuloso.valor_total += itens[i].valor;
            
            // imprime o item que foi adicionado
            printf("Adicionado item %d (Peso: %.2f, Valor: %.2f, Razão: %.2f)\n", itens[i].id, itens[i].peso, itens[i].valor, itens[i].razao);
        }
    }

    // Imprime o resultado final
    printf("\nResultado Guloso -> Valor Final: %.2f | Peso Final: %.2f\n", mochilaGuloso.valor_total, mochilaGuloso.peso_total);
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
        pesoTotal += lista[i].peso;
        printf("Item %d -> Peso: %.2f | Valor: %.2f | Razão: %.2f\n", i, lista[i].peso, lista[i].valor, lista[i].razao);
    }
    printf("Peso total dos itens: %d\n", pesoTotal);
}
