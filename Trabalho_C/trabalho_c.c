/*
 * Trabalho C — Atendimento Bancário (Tema C1)
 * Fila normal linear + fila preferencial circular, regra 2:1.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define ARQ_CSV "Trabalho_C\\dados_c.csv"

typedef struct SenhaNode {
    int senha;
    char tipo;
    char horario[6];
    struct SenhaNode *prox;
} SenhaNode;

SenhaNode *frontNormal = NULL;
SenhaNode *rearNormal = NULL;
int totalNormal = 0;

SenhaNode *rearPref = NULL;
int totalPref = 0;

int contadorNormal = 1;
int contadorPref = 1;

int totalAtendidosNormal = 0;
int totalAtendidosPref = 0;

void liberarFilas(void);

int lerInteiro(const char *mensagem) {
    char linha[50];
    int valor;
    int ok;

    do {
        printf("%s", mensagem);
        ok = 0;
        if (fgets(linha, sizeof(linha), stdin) == NULL) {
            printf("\n[INFO] Fim da entrada detectado. Encerrando o sistema.\n");
            return 0;
        }
        if (sscanf(linha, "%d", &valor) == 1) {
            ok = 1;
        }
        if (!ok) {
            printf("Entrada invalida! Digite um numero inteiro.\n");
        }
    } while (!ok);

    return valor;
}

char lerChar(const char *mensagem) {
    char linha[16];
    printf("%s", mensagem);
    if (fgets(linha, sizeof(linha), stdin) == NULL || strlen(linha) == 0) {
        return '\0';
    }
    return (char) toupper((unsigned char) linha[0]);
}

void obterHorarioAtual(char *buffer) {
    time_t t = time(NULL);
    struct tm *info = localtime(&t);
    strftime(buffer, 6, "%H:%M", info);
}

int filaNormalVazia(void) {
    return totalNormal == 0;
}

int filaNormalCheia(void) {
    return 0;
}

void enfileirarNormal(SenhaNode *node) {
    node->prox = NULL;
    if (filaNormalVazia()) {
        frontNormal = rearNormal = node;
    } else {
        rearNormal->prox = node;
        rearNormal = node;
    }
    totalNormal++;
}

SenhaNode *desenfileirarNormal(void) {
    if (filaNormalVazia()) return NULL;
    SenhaNode *node = frontNormal;
    frontNormal = frontNormal->prox;
    if (frontNormal == NULL) rearNormal = NULL;
    totalNormal--;
    return node;
}

void gerarSenhaNormal(void) {
    SenhaNode *node = (SenhaNode *)malloc(sizeof(SenhaNode));
    if (node == NULL) {
        printf("\n[ERRO] Memoria insuficiente para gerar senha normal.\n");
        return;
    }
    node->senha = contadorNormal++;
    node->tipo = 'N';
    obterHorarioAtual(node->horario);
    enfileirarNormal(node);
    printf("\nSenha N%03d gerada com sucesso! Horario de chegada: %s\n", node->senha, node->horario);
}

int cancelarNormal(int numero) {
    if (filaNormalVazia()) return 0;
    SenhaNode *atual = frontNormal;
    SenhaNode *anterior = NULL;
    while (atual != NULL) {
        if (atual->senha == numero) {
            if (anterior == NULL) {
                frontNormal = atual->prox;
                if (frontNormal == NULL) rearNormal = NULL;
            } else {
                anterior->prox = atual->prox;
                if (atual->prox == NULL) rearNormal = anterior;
            }
            free(atual);
            totalNormal--;
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    }
    return 0;
}

int filaPrefVazia(void) {
    return totalPref == 0;
}

int filaPrefCheia(void) {
    return 0;
}

/* Fila circular: rear->prox aponta para o primeiro nó */
void enfileirarPref(SenhaNode *node) {
    if (filaPrefVazia()) {
        node->prox = node;
        rearPref = node;
    } else {
        node->prox = rearPref->prox;
        rearPref->prox = node;
        rearPref = node;
    }
    totalPref++;
}

SenhaNode *desenfileirarPref(void) {
    if (filaPrefVazia()) return NULL;
    SenhaNode *front = rearPref->prox;
    if (front == rearPref) {
        rearPref = NULL;
    } else {
        rearPref->prox = front->prox;
    }
    totalPref--;
    return front;
}

void gerarSenhaPreferencial(void) {
    SenhaNode *node = (SenhaNode *)malloc(sizeof(SenhaNode));
    if (node == NULL) {
        printf("\n[ERRO] Memoria insuficiente para gerar senha preferencial.\n");
        return;
    }
    node->senha = contadorPref++;
    node->tipo = 'P';
    obterHorarioAtual(node->horario);
    enfileirarPref(node);
    printf("\nSenha P%03d gerada com sucesso! Horario de chegada: %s\n", node->senha, node->horario);
}

int cancelarPref(int numero) {
    if (filaPrefVazia()) return 0;
    SenhaNode *atual = rearPref->prox;
    SenhaNode *anterior = rearPref;
    do {
        if (atual->senha == numero) {
            if (atual->prox == atual) {
                rearPref = NULL;
            } else {
                if (atual == rearPref) {
                    rearPref = anterior;
                }
                anterior->prox = atual->prox;
            }
            free(atual);
            totalPref--;
            return 1;
        }
        anterior = atual;
        atual = atual->prox;
    } while (atual != rearPref->prox);
    return 0;
}

void consultarProximo(void) {
    printf("\n--- Proximo atendimento ---\n");
    if (filaNormalVazia() && filaPrefVazia()) {
        printf("[INFO] Nao ha nenhuma senha aguardando atendimento.\n");
        return;
    }

    static int normaisSeguidos = 0;

    if (!filaNormalVazia() && !filaPrefVazia()) {
        if (normaisSeguidos < 2) {
            printf("Proxima a ser atendida: N%03d (chegada: %s)\n",
                   frontNormal->senha, frontNormal->horario);
        } else {
            SenhaNode *front = rearPref->prox;
            printf("Proxima a ser atendida: P%03d (chegada: %s)\n",
                   front->senha, front->horario);
        }
    } else if (!filaNormalVazia()) {
        printf("Proxima a ser atendida: N%03d (chegada: %s)\n",
               frontNormal->senha, frontNormal->horario);
    } else {
        SenhaNode *front = rearPref->prox;
        printf("Proxima a ser atendida: P%03d (chegada: %s)\n",
               front->senha, front->horario);
    }
}

void atenderProxima(void) {
    static int normaisSeguidos = 0;
    SenhaNode *atendida = NULL;
    int atendeu = 0;

    if (filaNormalVazia() && filaPrefVazia()) {
        printf("\n[INFO] Nao ha nenhuma senha aguardando atendimento.\n");
        return;
    }

    if (!filaNormalVazia() && !filaPrefVazia()) {
        if (normaisSeguidos < 2) {
            atendida = desenfileirarNormal();
            normaisSeguidos++;
            totalAtendidosNormal++;
        } else {
            atendida = desenfileirarPref();
            normaisSeguidos = 0;
            totalAtendidosPref++;
        }
        atendeu = 1;
    } else if (!filaNormalVazia()) {
        atendida = desenfileirarNormal();
        totalAtendidosNormal++;
        atendeu = 1;
    } else {
        atendida = desenfileirarPref();
        totalAtendidosPref++;
        atendeu = 1;
    }

    if (atendeu) {
        printf("\n>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        printf(">>  Chamando senha: %c%03d\n", atendida->tipo, atendida->senha);
        printf(">>  Chegada: %s\n", atendida->horario);
        printf(">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\n");
        free(atendida);
    }
}

void buscarSenha(char tipo, int numero) {
    if (tipo == 'N') {
        int pos = 1;
        SenhaNode *atual = frontNormal;
        while (atual != NULL) {
            if (atual->senha == numero) {
                printf("\n[ENCONTRADA] N%03d | Chegada: %s | Posicao na fila: %d de %d\n",
                       atual->senha, atual->horario, pos, totalNormal);
                return;
            }
            atual = atual->prox;
            pos++;
        }
    } else if (tipo == 'P') {
        if (!filaPrefVazia()) {
            int pos = 1;
            SenhaNode *atual = rearPref->prox;
            do {
                if (atual->senha == numero) {
                    printf("\n[ENCONTRADA] P%03d | Chegada: %s | Posicao na fila: %d de %d\n",
                           atual->senha, atual->horario, pos, totalPref);
                    return;
                }
                atual = atual->prox;
                pos++;
            } while (atual != rearPref->prox);
        }
    } else {
        printf("\n[ERRO] Tipo invalido. Use N ou P.\n");
        return;
    }
    printf("\n[INFO] Senha %c%03d nao encontrada nas filas.\n", tipo, numero);
}

void listarFilas(void) {
    printf("\n===================== FILA NORMAL (total: %d) =====================\n", totalNormal);
    if (filaNormalVazia()) {
        printf("(fila vazia)\n");
    } else {
        int pos = 1;
        SenhaNode *atual = frontNormal;
        while (atual != NULL) {
            printf("  %2d) N%03d  | Chegada: %s\n", pos, atual->senha, atual->horario);
            atual = atual->prox;
            pos++;
        }
    }

    printf("\n================= FILA PREFERENCIAL (total: %d) =================\n", totalPref);
    if (filaPrefVazia()) {
        printf("(fila vazia)\n");
    } else {
        int pos = 1;
        SenhaNode *atual = rearPref->prox;
        do {
            printf("  %2d) P%03d  | Chegada: %s\n", pos, atual->senha, atual->horario);
            atual = atual->prox;
            pos++;
        } while (atual != rearPref->prox);
    }
    printf("\n");
}

void exibirRelatorio(void) {
    printf("\n===================== RELATORIO DO SISTEMA =====================\n");
    printf("Senhas normais geradas ate agora:        %d\n", contadorNormal - 1);
    printf("Senhas preferenciais geradas ate agora:   %d\n", contadorPref - 1);
    printf("Senhas normais atendidas:                 %d\n", totalAtendidosNormal);
    printf("Senhas preferenciais atendidas:            %d\n", totalAtendidosPref);
    printf("Senhas aguardando na fila normal:          %d\n", totalNormal);
    printf("Senhas aguardando na fila preferencial:    %d\n", totalPref);
    printf("==================================================================\n");
}

void salvarCSV(const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "w");
    if (f == NULL) {
        printf("\n[ERRO] Nao foi possivel abrir o arquivo '%s' para escrita.\n", nomeArquivo);
        return;
    }

    fprintf(f, "senha;tipo;horario\n");

    SenhaNode *atual = frontNormal;
    while (atual != NULL) {
        fprintf(f, "%d;%c;%s\n", atual->senha, atual->tipo, atual->horario);
        atual = atual->prox;
    }

    if (!filaPrefVazia()) {
        atual = rearPref->prox;
        do {
            fprintf(f, "%d;%c;%s\n", atual->senha, atual->tipo, atual->horario);
            atual = atual->prox;
        } while (atual != rearPref->prox);
    }

    fclose(f);
    printf("\n[OK] Filas salvas com sucesso em '%s'.\n", nomeArquivo);
}

void carregarCSV(const char *nomeArquivo) {
    FILE *f = fopen(nomeArquivo, "r");
    if (f == NULL) {
        printf("\n[ERRO] Arquivo '%s' nao encontrado.\n", nomeArquivo);
        return;
    }

    liberarFilas();
    contadorNormal = 1;
    contadorPref = 1;

    char linha[100];
    int primeiraLinha = 1;

    while (fgets(linha, sizeof(linha), f) != NULL) {
        linha[strcspn(linha, "\r\n")] = '\0';

        if (primeiraLinha) {
            primeiraLinha = 0;
            continue;
        }
        if (strlen(linha) == 0) continue;

        char *token = strtok(linha, ";");
        if (token == NULL) continue;
        int senha = atoi(token);

        token = strtok(NULL, ";");
        if (token == NULL) continue;
        char tipo = (char) toupper((unsigned char) token[0]);

        token = strtok(NULL, ";");
        if (token == NULL) continue;

        SenhaNode *node = (SenhaNode *)malloc(sizeof(SenhaNode));
        if (node == NULL) {
            printf("\n[ERRO] Memoria insuficiente ao carregar dados.\n");
            break;
        }

        node->senha = senha;
        node->tipo = tipo;
        strncpy(node->horario, token, 5);
        node->horario[5] = '\0';

        if (tipo == 'N') {
            enfileirarNormal(node);
            if (senha >= contadorNormal) contadorNormal = senha + 1;
        } else if (tipo == 'P') {
            enfileirarPref(node);
            if (senha >= contadorPref) contadorPref = senha + 1;
        } else {
            free(node);
        }
    }

    fclose(f);
    printf("\n[OK] Filas carregadas a partir de '%s'.\n", nomeArquivo);
}

void liberarFilas(void) {
    while (!filaNormalVazia()) {
        SenhaNode *node = desenfileirarNormal();
        free(node);
    }
    while (!filaPrefVazia()) {
        SenhaNode *node = desenfileirarPref();
        free(node);
    }
}

void exibirMenu(void) {
    printf("\n=========================================\n");
    printf("   SISTEMA DE ATENDIMENTO BANCARIO\n");
    printf("=========================================\n");
    printf(" 1 - Gerar senha NORMAL\n");
    printf(" 2 - Gerar senha PREFERENCIAL\n");
    printf(" 3 - Chamar proxima senha (atendimento)\n");
    printf(" 4 - Consultar proxima senha (peek)\n");
    printf(" 5 - Buscar senha\n");
    printf(" 6 - Listar filas\n");
    printf(" 7 - Cancelar senha\n");
    printf(" 8 - Salvar filas em arquivo CSV\n");
    printf(" 9 - Carregar filas de arquivo CSV\n");
    printf("10 - Relatorio / Estatisticas\n");
    printf(" 0 - Sair\n");
    printf("=========================================\n");
}

int main(void) {
    int opcao;

    printf("Bem-vindo ao Sistema de Atendimento Bancario!\n");

    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao: ");

        switch (opcao) {
            case 1:
                gerarSenhaNormal();
                break;

            case 2:
                gerarSenhaPreferencial();
                break;

            case 3:
                atenderProxima();
                break;

            case 4:
                consultarProximo();
                break;

            case 5: {
                char tipo = lerChar("Digite o tipo da senha (N/P): ");
                int numero = lerInteiro("Digite o numero da senha: ");
                buscarSenha(tipo, numero);
                break;
            }

            case 6:
                listarFilas();
                break;

            case 7: {
                char tipo = lerChar("Digite o tipo da senha a cancelar (N/P): ");
                int numero = lerInteiro("Digite o numero da senha: ");
                int ok = 0;

                if (tipo == 'N') {
                    ok = cancelarNormal(numero);
                } else if (tipo == 'P') {
                    ok = cancelarPref(numero);
                } else {
                    printf("\n[ERRO] Tipo invalido. Use N ou P.\n");
                    break;
                }

                if (ok) {
                    printf("\n[OK] Senha %c%03d cancelada com sucesso.\n", tipo, numero);
                } else {
                    printf("\n[INFO] Senha %c%03d nao encontrada.\n", tipo, numero);
                }
                break;
            }

            case 8:
                salvarCSV(ARQ_CSV);
                break;

            case 9:
                carregarCSV(ARQ_CSV);
                break;

            case 10:
                exibirRelatorio();
                break;

            case 0:
                printf("\nEncerrando o sistema. Ate logo!\n");
                break;

            default:
                printf("\n[ERRO] Opcao invalida! Tente novamente.\n");
        }

    } while (opcao != 0);

    liberarFilas();
    return 0;
}
