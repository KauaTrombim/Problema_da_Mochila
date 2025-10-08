#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_VALOR 10
#define MAX_VALOR 100

// --- Estruturas de Dados ---
typedef struct {
    int id;
    int peso;
    int valor;
    double razao; // valor / peso
} Item;

typedef struct {
    int max_w;
    long long valor_total;
    int peso_total;
} Mochila;

// --- Declarações de Funções ---
void criarItens_NaoCorrelacionado(Item* lista, int n, int w);
void criarItens_CorrelacionadoComRuido(Item* lista, int n, int w);
void criarItens_FortementeCorrelacionado(Item* lista, int n, int w);
void lerItens(Item* lista, int n);
void imprimirItens(Item* lista, int n);
void guloso(Item *itens, int n, int w);
void forca_bruta_recursiva(Item* itens, int n, int w, long long valAtual, int pesoAtual, int i, long long* maxVal, long long comb, long long* bestComb);
void forca_bruta(Item *itens, int n, int w);

// --- Programa Principal ---
int main() {
    int n; // Número de itens
    int w; // Peso máximo da mochila
    int a; // Seletor de algoritmo
    int s; // Seleciona entrada de itens
    int t; // Tipo de correlação

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

    if (s == 1){
        lerItens(itens, n);
    }else if (s == 2){

        printf("\nSelecione o tipo de geração de itens:\n");
        printf("1. Pesos e Valores Não Correlacionados\n");
        printf("2. Pesos e Valores Correlacionados com Ruído\n");
        printf("3. Pesos e Valores Fortemente Correlacionados\n");
        scanf("%d", &t);

        // Chama a função de criação apropriada com base na escolha do usuário
        switch (t) {
            case 1:
                criarItens_NaoCorrelacionado(itens, n, w);
                break;
            case 2:
                criarItens_CorrelacionadoComRuido(itens, n, w);
                break;
            case 3:
                criarItens_FortementeCorrelacionado(itens, n, w);
                break;
        }
        imprimirItens(itens, n);
    }


    if(a == 1){
        forca_bruta(itens, n, w);
    }else if(a == 2){
        guloso(itens, n, w); 
    }

    return 0;
}

void forca_bruta(Item *itens, int n, int w){
    if (n >= 64) {
    printf("Erro: O número de itens deve ser menor que 64 para a solução de força bruta.\n");
    return;
    }
    // --- 2. Algoritmo de Força Bruta ---
    printf("\n--- Executando Solução Força Bruta ---\n");
    if (n > 30) {
        printf("(Aviso: n > 30, isso pode levar muito tempo...)\n");
    }

    long long maxVal = 0;
    long long bestComb = 0LL;

    forca_bruta_recursiva(itens, n, (double)w, 0, 0, 0, &maxVal, 0LL, &bestComb);

    printf("Itens na melhor combinação:\n");

    int pesoFinal = 0;
    long long valorFinal = 0;

    if (bestComb == 0 && maxVal == 0) {
        printf("  - Nenhum item foi selecionado.\n");
    } else {
    for (int i = 0; i < n; i++) {
        if ((bestComb >> i) & 1) {
            printf("Adicionado Item %d (Peso: %d, Valor: %d)\n", itens[i].id, itens[i].peso, itens[i].valor);
                pesoFinal += itens[i].peso;
                valorFinal += itens[i].valor;
            }
         }
    }
        printf("\nResultado Força Bruta -> Valor Final: %lld | Peso Final: %d\n", valorFinal, pesoFinal);
}

void forca_bruta_recursiva(Item* itens, int n, int w,
                           long long valAtual, int pesoAtual, int i,
                           long long* maxVal, long long comb, long long* bestComb)
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
    forca_bruta_recursiva(itens, n, w, valAtual, pesoAtual, i + 1, maxVal, comb, bestComb);

    // --- Inclui o item atual ---
    double novoPeso = pesoAtual + itens[i].peso;
    double novoValor = valAtual + itens[i].valor;

    // Só explora se ainda há chance de ser válido (Árvore com "poda", já que não explora toda a árvore)
    if (novoPeso <= w) {
        forca_bruta_recursiva(itens, n, w, novoValor, novoPeso, i + 1, maxVal, comb | (1LL << i), bestComb);
    }
}

// --- Função de comparação para qsort ---
// --- Ordena os itens em ordem decrescente com base na sua razão (valor/peso) ---
int compareItens(const void *a, const void *b) {
    // Converte os ponteiros void* para ponteiros do tipo Item*
    Item *itemA = (Item *)a;
    Item *itemB = (Item *)b;

    // Compara as razões para ordenação decrescente
    if (itemA->razao < itemB->razao) return 1;
    if (itemA->razao > itemB->razao) return -1;
    return 0; // São iguais
}

void guloso(Item* itens, int n, int w){
    qsort(itens, n, sizeof(Item), compareItens);
  
    Mochila mochilaGuloso = { .max_w = w, .valor_total = 0, .peso_total = 0 };

    printf("\n--- Executando Solução Gulosa ---\n");

    // Itera sobre os itens já ordenados e preenche a mochila
    for (int i = 0; i < n; i++) {
        if (mochilaGuloso.peso_total + itens[i].peso <= mochilaGuloso.max_w) {
            mochilaGuloso.peso_total += itens[i].peso;
            mochilaGuloso.valor_total += itens[i].valor;
            
            // imprime o item que foi adicionado
            printf("Adicionado item %d (Peso: %d, Valor: %d, Razão: %.2f)\n", itens[i].id, itens[i].peso, itens[i].valor, itens[i].razao);
        }
    }

    // Imprime o resultado final
    printf("\nResultado Guloso -> Valor Final: %lld | Peso Final: %d\n", mochilaGuloso.valor_total, mochilaGuloso.peso_total);
}

///////////////// Funções Utilitárias /////////////////

void criarItens_NaoCorrelacionado(Item* lista, int n, int w) {
    int min_peso = 1;
    int max_peso = (int)(w * 0.4);

    for (int i = 0; i < n; i++) {
        lista[i].id = i;
        lista[i].peso = (rand() % (max_peso - min_peso + 1)) + min_peso;
        lista[i].valor = (rand() % (MAX_VALOR - MIN_VALOR + 1)) + MIN_VALOR;
        if (lista[i].peso > 0) {
            lista[i].razao = (double)lista[i].valor / lista[i].peso;
        } else lista[i].razao = 0;
    }
}

void criarItens_CorrelacionadoComRuido(Item* lista, int n, int w) {
    int min_peso = 1;
    int max_peso = (int)(w * 0.4);
    if (max_peso < min_peso) max_peso = min_peso;

    for (int i = 0; i < n; i++) {
        lista[i].id = i;
        lista[i].peso = (rand() % (max_peso - min_peso + 1)) + min_peso;

        // O valor base é 2x o peso.
        int valor_base = lista[i].peso * 2;
        // O ruído é uma variação de até 50% do valor base (para cima ou para baixo).
        int faixa_ruido = (int)(valor_base * 0.5);
        int ruido = 0;
        if (faixa_ruido > 0) {
            ruido = (rand() % (2 * faixa_ruido + 1)) - faixa_ruido; // Ruído entre [-faixa, +faixa]
        }
        
        lista[i].valor = valor_base + ruido;

        // Garante que o valor final seja pelo menos 1.
        if (lista[i].valor < 1) lista[i].valor = 1;

        if (lista[i].peso > 0) lista[i].razao = (double)lista[i].valor / lista[i].peso;
        else lista[i].razao = 0;
    }
}

void criarItens_FortementeCorrelacionado(Item* lista, int n, int w) {
    int min_peso = 1;
    int max_peso = (int)(w * 0.4);
    if (max_peso < min_peso) max_peso = min_peso;

    for (int i = 0; i < n; i++) {
        lista[i].id = i;
        lista[i].peso = (rand() % (max_peso - min_peso + 1)) + min_peso;

        // O valor é 2x o peso, mais uma pequena variação aleatória de 0 a 15.
        int valor_base = lista[i].peso * 2;
        int ruido_pequeno = rand() % 16; // Ruído pequeno e sempre positivo [0, 15]
        lista[i].valor = valor_base + ruido_pequeno;

        if (lista[i].peso > 0) lista[i].razao = (double)lista[i].valor / lista[i].peso;
        else lista[i].razao = 0;
    }
}

void lerItens(Item* lista, int n) {
    printf("\nInsira os itens (peso valor) em cada linha:\n");

    for (int i = 0; i < n; i++) {
        int peso, valor;

        printf("Item %d -> ", i);
        if (scanf("%d %d", &peso, &valor) != 2) {
            printf("Entrada inválida! Tente novamente.\n");
            i--; // volta uma iteração para repetir a leitura
            while (getchar() != '\n'); // limpa buffer
            continue;
        }

        lista[i].id = i;
        lista[i].peso = peso;
        lista[i].valor = valor;
        lista[i].razao = (peso > 0) ? ((double)valor / peso) : 0.0;
    }

    printf("\nItens adicionados com sucesso!\n");
    imprimirItens(lista, n);
}


void imprimirItens(Item* lista, int n) {
    int pesoTotal = 0;
    for (int i = 0; i < n; i++) {
        pesoTotal += lista[i].peso;
        printf("Item %d -> Peso: %d | Valor: %d | Razão: %.2f\n", i, lista[i].peso, lista[i].valor, lista[i].razao);
    }
    printf("Peso total dos itens: %d\n", pesoTotal);
}
