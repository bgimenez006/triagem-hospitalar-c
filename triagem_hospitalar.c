// Sistema de Triagem Hospitalar
// Uso de filas com prioridade
// Autor: Beatriz Gimenez

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HIST 3

// ================= ESTRUTURAS =================

typedef struct Paciente {
    char nome[50];
    int prioridade;
    struct Paciente *prox;
} Paciente;

typedef struct {
    Paciente *inicio;
    Paciente *fim;
    int tamanho;
} Fila;

// ================= FILA =================

void inicializarFila(Fila *f) {
    f->inicio = NULL;
    f->fim = NULL;
    f->tamanho = 0;
}

int filaVazia(Fila *f) {
    return f->inicio == NULL;
}

void enfileirar(Fila *f, char nome[], int prioridade) {
    Paciente *novo = (Paciente *)malloc(sizeof(Paciente));
    strcpy(novo->nome, nome);
    novo->prioridade = prioridade;
    novo->prox = NULL;

    if (filaVazia(f)) {
        f->inicio = novo;
    } else {
        f->fim->prox = novo;
    }

    f->fim = novo;
    f->tamanho++;
}

Paciente* desenfileirar(Fila *f) {
    if (filaVazia(f)) r
        eturn NULL;

    Paciente *temp = f->inicio;
    f->inicio = temp->prox;

    if (f->inicio == NULL) f->fim = NULL;

    f->tamanho--;
    return temp;
}

// ================= AUXILIO =================

char* nomePrioridade(int p) {
    switch (p) {
        case 1: return "VERMELHO";
        case 2: return "AMARELO";
        case 3: return "VERDE";
        case 4: return "AZUL";
        default: return "";
    }
}

// ================= HISTÓRICO =================

void adicionarHistorico(char historico[][100], char texto[]) {
    for (int i = MAX_HIST - 1; i > 0; i--) {
        strcpy(historico[i], historico[i - 1]);
    }
    strcpy(historico[0], texto);
}

// ================= REMOVER POR NOME =================

void removerPaciente(Fila *f, char nome[]) {
    Paciente *atual = f->inicio;
    Paciente *anterior = NULL;

    while (atual != NULL) {
        if (strcmp(atual->nome, nome) == 0) {
            if (anterior == NULL)
                f->inicio = atual->prox;
            else
                anterior->prox = atual->prox;

            if (atual == f->fim)
                f->fim = anterior;

            free(atual);
            f->tamanho--;
            printf("Paciente removido com sucesso!\n");
            return;
        }

        anterior = atual;
        atual = atual->prox;
    }

    printf("Paciente nao encontrado.\n");
}

// ================= INTERFACE =================

void mostrarPainel(char atual[], char historico[][100],
                   Fila *v, Fila *a, Fila *vd, Fila *az,
                   int atendidos) {

    printf("\n=====================================\n");
    printf("        PAINEL DE ATENDIMENTO\n");
    printf("=====================================\n");

    if (strlen(atual) > 0)
        printf("\n>>> %s <<<\n", atual);
    else
        printf("\n>>> AGUARDANDO PACIENTES <<<\n");

    printf("\n--- Ultimos chamados ---\n");
    for (int i = 0; i < MAX_HIST; i++) {
        if (strlen(historico[i]) > 0)
            printf("%d. %s\n", i + 1, historico[i]);
    }

    printf("\n--- Filas ---\n");
    printf("Vermelho: %d\n", v->tamanho);
    printf("Amarelo : %d\n", a->tamanho);
    printf("Verde   : %d\n", vd->tamanho);
    printf("Azul    : %d\n", az->tamanho);

    printf("\nTotal atendidos: %d\n", atendidos);
    printf("=====================================\n");
}

// ================= MAIN =================

int main() {
    Fila vermelho, amarelo, verde, azul;
    inicializarFila(&vermelho);
    inicializarFila(&amarelo);
    inicializarFila(&verde);
    inicializarFila(&azul);

    char historico[MAX_HIST][100] = {""};
    char atual[100] = "";

    int opcao, prioridade;
    char nome[50];
    int atendidos = 0;

    int contV = 0, contA = 0, contVD = 0, contAZ = 0;

    do {
        mostrarPainel(atual, historico, &vermelho, &amarelo, &verde, &azul, atendidos);

        printf("\n1 - Adicionar paciente\n");
        printf("2 - Chamar pro atendimento\n");
        printf("3 - Remover paciente\n");
        printf("4 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {

            case 1:
                printf("Nome do paciente: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;

                printf("Prioridade (1- Vermelho | 2- Amarelo | 3- Verde | 4- Azul): ");
                scanf("%d", &prioridade);

                switch (prioridade) {
                    case 1: enfileirar(&vermelho, nome, prioridade); break;
                    case 2: enfileirar(&amarelo, nome, prioridade); break;
                    case 3: enfileirar(&verde, nome, prioridade); break;
                    case 4: enfileirar(&azul, nome, prioridade); break;
                    default: printf("Opcao invalida!\n");
                }
                break;

            case 2: {
                Paciente *p = NULL;

                if (!filaVazia(&vermelho)) p = desenfileirar(&vermelho);
                else if (!filaVazia(&amarelo)) p = desenfileirar(&amarelo);
                else if (!filaVazia(&verde)) p = desenfileirar(&verde);
                else if (!filaVazia(&azul)) p = desenfileirar(&azul);

                if (p != NULL) {
                    sprintf(atual, "CHAMANDO: %s (%s)",
                            p->nome, nomePrioridade(p->prioridade));

                    adicionarHistorico(historico, atual);

                    atendidos++;

                    if (p->prioridade == 1) contV++;
                    if (p->prioridade == 2) contA++;
                    if (p->prioridade == 3) contVD++;
                    if (p->prioridade == 4) contAZ++;

                    free(p);
                } else {
                    printf("Nao ha pacientes!\n");
                }
                break;
            }

            case 3:
                printf("Nome para remover: ");
                fgets(nome, 50, stdin);
                nome[strcspn(nome, "\n")] = 0;

                removerPaciente(&vermelho, nome);
                removerPaciente(&amarelo, nome);
                removerPaciente(&verde, nome);
                removerPaciente(&azul, nome);
                break;

            case 4:
                printf("\n===== RELATORIO FINAL =====\n");
                printf("Total atendidos: %d\n", atendidos);
                printf("Vermelho: %d\n", contV);
                printf("Amarelo : %d\n", contA);
                printf("Verde   : %d\n", contVD);
                printf("Azul    : %d\n", contAZ);
                printf("Encerrando...\n");
                break;

            default:
                printf("Opcao invalida!\n");
        }

    } while (opcao != 4);

    return 0;
}
