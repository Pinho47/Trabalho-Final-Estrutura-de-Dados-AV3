/*
 * Trabalho B — Bandeja de E-mails (Tema B3)
 * Pilha encadeada com alocação dinâmica (LIFO).
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_STR    100
#define TAM_CORPO  300
#define ARQUIVO_CSV "Trabalho_B\\dados_b.csv"

#define COR_RESET   "\x1b[0m"
#define COR_TITULO  "\x1b[1;34m"
#define COR_MENU    "\x1b[1;33m"
#define COR_DADO    "\x1b[0;34m"
#define COR_AVISO   "\x1b[1;33m"

typedef struct EmailNode {
    int id;
    char remetente[TAM_STR];
    char assunto[TAM_STR];
    char data[TAM_STR];
    char categoria[TAM_STR];
    char corpo[TAM_CORPO];
    struct EmailNode *prox;
} EmailNode;

EmailNode *topo = NULL;
EmailNode *topo_aux = NULL;

int proximo_id = 1;

int pilha_vazia() { return topo == NULL; }
int aux_vazia()   { return topo_aux == NULL; }

void limpar_tela() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pausar() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

int ler_inteiro(const char *prompt, int *destino) {
    char buf[32];
    printf("%s", prompt);
    if (!fgets(buf, sizeof(buf), stdin)) return 0;
    char *fim;
    long val = strtol(buf, &fim, 10);
    if (fim == buf || (*fim != '\n' && *fim != '\0')) {
        printf(COR_AVISO "  [ERRO] Digite apenas um numero inteiro.\n" COR_RESET);
        return 0;
    }
    *destino = (int)val;
    return 1;
}

int ler_texto(const char *prompt, char *destino, int tamanho) {
    printf("%s", prompt);
    if (!fgets(destino, tamanho, stdin)) return 0;
    destino[strcspn(destino, "\n")] = '\0';
    if (destino[0] == '\0') {
        printf(COR_AVISO "  [ERRO] Campo nao pode ser vazio.\n" COR_RESET);
        return 0;
    }
    return 1;
}

int validar_data(const char *data) {
    int d, m, a, h, min;
    if (sscanf(data, "%d/%d/%d %d:%d", &d, &m, &a, &h, &min) != 5) return 0;
    if (d < 1 || d > 31) return 0;
    if (m < 1 || m > 12) return 0;
    if (a < 1900 || a > 9999) return 0;
    if (h < 0 || h > 23) return 0;
    if (min < 0 || min > 59) return 0;
    return 1;
}

void aux_empilhar(EmailNode *no) {
    no->prox = topo_aux;
    topo_aux = no;
}

EmailNode* aux_desempilhar() {
    if (topo_aux == NULL) return NULL;
    EmailNode *temp = topo_aux;
    topo_aux = topo_aux->prox;
    return temp;
}

void copiar_campo_seguro(char *destino, const char *origem, size_t tamanhoDestino) {
    strncpy(destino, origem, tamanhoDestino - 1);
    destino[tamanhoDestino - 1] = '\0';
}

void receber_email(const char *remetente, const char *assunto,
                   const char *data, const char *categoria,
                   const char *corpo) {
    EmailNode *novo = (EmailNode *)malloc(sizeof(EmailNode));
    if (novo == NULL) {
        printf(COR_AVISO "  [ERRO] Memoria insuficiente!\n" COR_RESET);
        return;
    }
    novo->id = proximo_id++;
    copiar_campo_seguro(novo->remetente, remetente, TAM_STR);
    copiar_campo_seguro(novo->assunto,   assunto,   TAM_STR);
    copiar_campo_seguro(novo->data,      data,      TAM_STR);
    copiar_campo_seguro(novo->categoria, categoria, TAM_STR);
    copiar_campo_seguro(novo->corpo,     corpo,     TAM_CORPO);
    novo->prox = topo;
    topo = novo;
}

void exibir_email(EmailNode e) {
    printf(COR_TITULO "  +==================================================+\n" COR_RESET);
    printf(COR_TITULO "  |                    E-MAIL                       |\n" COR_RESET);
    printf(COR_TITULO "  +==================================================+\n\n" COR_RESET);
    printf(COR_MENU "  ID        : " COR_RESET COR_DADO "%d\n" COR_RESET,  e.id);
    printf(COR_MENU "  De        : " COR_RESET COR_DADO "%s\n" COR_RESET,  e.remetente);
    printf(COR_MENU "  Assunto   : " COR_RESET COR_DADO "%s\n" COR_RESET,  e.assunto);
    printf(COR_MENU "  Data      : " COR_RESET COR_DADO "%s\n" COR_RESET,  e.data);
    printf(COR_TITULO "\n  -- Mensagem ---------------------------------------\n" COR_RESET);
    printf(COR_DADO "  %s\n" COR_RESET, e.corpo);
    printf(COR_TITULO "  -----------------------------------------------\n" COR_RESET);
}

void peek_email() {
    limpar_tela();
    if (pilha_vazia()) {
        printf(COR_AVISO "  Bandeja vazia.\n" COR_RESET);
        pausar();
        return;
    }
    printf(COR_TITULO "  +================================+\n" COR_RESET);
    printf(COR_TITULO "  |   E-MAIL NO TOPO (SEM REMOVER) |\n" COR_RESET);
    printf(COR_TITULO "  +================================+\n\n" COR_RESET);
    exibir_email(*topo);
    pausar();
}

void ler_proximo_email() {
    limpar_tela();
    if (pilha_vazia()) {
        printf(COR_AVISO "  Nenhum e-mail nao lido.\n" COR_RESET);
        pausar();
        return;
    }
    exibir_email(*topo);
    EmailNode *temp = topo;
    topo = topo->prox;
    free(temp);
    pausar();
}

/* Marca como lido: desempilha da principal para auxiliar até encontrar o ID,
 * libera o nó, depois devolve tudo à pilha original */
void marcar_como_lido(int id) {
    limpar_tela();
    if (pilha_vazia()) {
        printf(COR_AVISO "  Bandeja vazia.\n" COR_RESET);
        pausar();
        return;
    }

    int encontrado = 0;

    while (!pilha_vazia()) {
        EmailNode *e = topo;
        topo = topo->prox;
        if (e->id == id) {
            encontrado = 1;
            exibir_email(*e);
            free(e);
            break;
        }
        aux_empilhar(e);
    }

    if (!encontrado)
        printf(COR_AVISO "  E-mail com ID %d nao encontrado.\n" COR_RESET, id);

    while (!aux_vazia()) {
        EmailNode *e = aux_desempilhar();
        e->prox = topo;
        topo = e;
    }
    pausar();
}

void excluir_email(int id) {
    limpar_tela();
    if (pilha_vazia()) {
        printf(COR_AVISO "  Bandeja vazia. Nada para excluir.\n" COR_RESET);
        pausar();
        return;
    }

    int encontrado = 0;
    EmailNode *alvo = NULL;

    while (!pilha_vazia()) {
        EmailNode *e = topo;
        topo = topo->prox;
        if (e->id == id) {
            encontrado = 1;
            alvo = e;
            break;
        }
        aux_empilhar(e);
    }

    if (!encontrado) {
        printf(COR_AVISO "  E-mail com ID %d nao encontrado.\n" COR_RESET, id);
    } else {
        printf(COR_TITULO "  -- Confirmar exclusao ----------------------\n" COR_RESET);
        printf(COR_MENU "  De      : " COR_RESET COR_DADO "%s\n" COR_RESET, alvo->remetente);
        printf(COR_MENU "  Assunto : " COR_RESET COR_DADO "%s\n" COR_RESET, alvo->assunto);

        char linha[16];
        char confirmacao = '\0';
        printf(COR_AVISO "  Tem certeza que deseja excluir este e-mail? (S/N): " COR_RESET);
        if (fgets(linha, sizeof(linha), stdin) != NULL) {
            confirmacao = linha[0];
        }

        if (confirmacao == 'S' || confirmacao == 's') {
            printf(COR_DADO "  E-mail excluido com sucesso!\n" COR_RESET);
            free(alvo);
        } else {
            printf(COR_AVISO "  Exclusao cancelada.\n" COR_RESET);
            alvo->prox = topo;
            topo = alvo;
        }
    }

    while (!aux_vazia()) {
        EmailNode *e = aux_desempilhar();
        e->prox = topo;
        topo = e;
    }
    pausar();
}

void buscar_email() {
    limpar_tela();
    if (pilha_vazia()) {
        printf(COR_AVISO "  Bandeja vazia.\n" COR_RESET);
        pausar();
        return;
    }

    printf(COR_TITULO "  +================================+\n" COR_RESET);
    printf(COR_TITULO "  |          BUSCAR E-MAIL           |\n" COR_RESET);
    printf(COR_TITULO "  +================================+\n" COR_RESET);
    printf(COR_MENU    "  |  1. Buscar por ID                |\n" COR_RESET);
    printf(COR_MENU    "  |  2. Buscar por remetente         |\n" COR_RESET);
    printf(COR_TITULO "  +================================+\n\n" COR_RESET);

    int opcao;
    if (!ler_inteiro(COR_MENU "  Opcao: " COR_RESET, &opcao)) { pausar(); return; }

    int ids_encontrados[50];
    int encontrados = 0;

    if (opcao == 1) {
        int id;
        if (!ler_inteiro(COR_MENU "  ID: " COR_RESET, &id)) { pausar(); return; }
        limpar_tela();
        for (EmailNode *p = topo; p != NULL; p = p->prox) {
            if (p->id == id) {
                exibir_email(*p);
                ids_encontrados[encontrados++] = p->id;
                break;
            }
        }
    } else if (opcao == 2) {
        char termo[TAM_STR];
        if (!ler_texto(COR_MENU "  Remetente (ou parte): " COR_RESET, termo, TAM_STR)) { pausar(); return; }
        limpar_tela();
        printf(COR_MENU "  Resultados para \"%s\":\n\n" COR_RESET, termo);
        for (EmailNode *p = topo; p != NULL; p = p->prox) {
            if (strstr(p->remetente, termo) != NULL) {
                exibir_email(*p);
                ids_encontrados[encontrados++] = p->id;
            }
        }
    } else {
        printf(COR_AVISO "  Opcao invalida.\n" COR_RESET);
        pausar();
        return;
    }

    if (encontrados == 0) {
        printf(COR_AVISO "  Nenhum e-mail encontrado.\n" COR_RESET);
        pausar();
        return;
    }

    printf(COR_DADO "\n  %d e-mail(is) encontrado(s).\n" COR_RESET, encontrados);

    int confirmar;
    if (!ler_inteiro(COR_MENU "\n  Deseja marcar como lido? (1 = Sim / 0 = Nao): " COR_RESET, &confirmar)) { pausar(); return; }
    if (confirmar != 1) { pausar(); return; }

    int id_alvo;
    if (encontrados == 1) {
        id_alvo = ids_encontrados[0];
    } else {
        if (!ler_inteiro(COR_MENU "  Informe o ID do e-mail a marcar como lido: " COR_RESET, &id_alvo)) { pausar(); return; }
    }

    marcar_como_lido(id_alvo);
}

void exibir_bandeja() {
    limpar_tela();
    printf(COR_TITULO "  +============================================================+\n" COR_RESET);
    printf(COR_TITULO "  |                   BANDEJA DE ENTRADA                       |\n" COR_RESET);
    printf(COR_TITULO "  +============================================================+\n\n" COR_RESET);

    if (pilha_vazia()) {
        printf(COR_AVISO "  Nenhum e-mail nao lido.\n" COR_RESET);
        pausar();
        return;
    }

    printf(COR_MENU "  %-4s  %-22s  %-28s  %-16s\n" COR_RESET,
           "ID", "Remetente", "Assunto", "Data");
    printf(COR_TITULO "  ---------------------------------------------------------------\n" COR_RESET);
    int total = 0;
    for (EmailNode *p = topo; p != NULL; p = p->prox) {
        printf(COR_DADO "  %-4d  %-22s  %-28s  %-16s\n" COR_RESET,
               p->id,
               p->remetente,
               p->assunto,
               p->data);
        total++;
    }
    printf(COR_TITULO "  ---------------------------------------------------------------\n" COR_RESET);
    printf(COR_MENU "  Total: %d e-mail(is) nao lido(s)\n" COR_RESET, total);
    pausar();
}

void simular_recebimento() {
    limpar_tela();
    printf(COR_TITULO "  -- Simular Recebimento de E-mail -------------------\n\n" COR_RESET);

    char remetente[TAM_STR], assunto[TAM_STR], data[TAM_STR];
    char categoria[TAM_STR], corpo[TAM_CORPO];

    if (!ler_texto(COR_MENU "  Remetente  : " COR_RESET, remetente, TAM_STR))  { pausar(); return; }
    if (!ler_texto(COR_MENU "  Assunto    : " COR_RESET, assunto,   TAM_STR))  { pausar(); return; }
    if (!ler_texto(COR_MENU "  Categoria  : " COR_RESET, categoria, TAM_STR))  { pausar(); return; }

    do {
        if (!ler_texto(COR_MENU "  Data (DD/MM/AAAA HH:MM): " COR_RESET, data, TAM_STR)) { pausar(); return; }
        if (!validar_data(data))
            printf(COR_AVISO "  [ERRO] Data invalida. Use o formato DD/MM/AAAA HH:MM.\n" COR_RESET);
    } while (!validar_data(data));

    if (!ler_texto(COR_MENU "  Mensagem   : " COR_RESET, corpo, TAM_CORPO)) { pausar(); return; }

    receber_email(remetente, assunto, data, categoria, corpo);
    printf(COR_DADO "\n  E-mail simulado recebido com sucesso! (ID %d)\n" COR_RESET, proximo_id - 1);
    pausar();
}

void escreverCampoCSV(FILE *arquivo, const char *campo) {
    int precisaAspas = (strchr(campo, ';') != NULL ||
                         strchr(campo, '"')  != NULL ||
                         strchr(campo, '\n') != NULL);

    if (precisaAspas) {
        fputc('"', arquivo);
        for (const char *p = campo; *p != '\0'; p++) {
            if (*p == '"') fputc('"', arquivo);
            fputc(*p, arquivo);
        }
        fputc('"', arquivo);
    } else {
        fputs(campo, arquivo);
    }
}

int lerCampoCSV(FILE *arquivo, char *destino, int tamanho) {
    int c = fgetc(arquivo);
    if (c == EOF) { destino[0] = '\0'; return -1; }

    int comAspas = 0, i = 0;
    if (c == '"') comAspas = 1;
    else ungetc(c, arquivo);

    while (1) {
        c = fgetc(arquivo);
        if (c == EOF) { destino[i] = '\0'; return -1; }

        if (comAspas) {
            if (c == '"') {
                int prox = fgetc(arquivo);
                if (prox == '"') {
                    if (i < tamanho - 1) destino[i++] = '"';
                } else {
                    ungetc(prox, arquivo);
                    comAspas = 0;
                }
            } else if (i < tamanho - 1) {
                destino[i++] = (char)c;
            }
        } else {
            if (c == '\r') continue;
            if (c == ';' || c == '\n') { destino[i] = '\0'; return (c == '\n'); }
            if (i < tamanho - 1) destino[i++] = (char)c;
        }
    }
}

void salvarCSV() {
    FILE *arquivo = fopen(ARQUIVO_CSV, "w");
    if (arquivo == NULL) {
        printf(COR_AVISO "  [ERRO] Nao foi possivel salvar o arquivo.\n" COR_RESET);
        return;
    }

    fprintf(arquivo, "id;remetente;assunto;data\n");

    /* Inverte antes de salvar para manter ordem cronológica */
    EmailNode *reverso = NULL;
    EmailNode *p = topo;
    while (p != NULL) {
        EmailNode *prox = p->prox;
        p->prox = reverso;
        reverso = p;
        p = prox;
    }
    topo = reverso;

    for (EmailNode *q = topo; q != NULL; q = q->prox) {
        fprintf(arquivo, "%d;", q->id);
        escreverCampoCSV(arquivo, q->remetente); fputc(';', arquivo);
        escreverCampoCSV(arquivo, q->assunto);   fputc(';', arquivo);
        escreverCampoCSV(arquivo, q->data);
        fputc('\n', arquivo);
    }

    reverso = NULL;
    p = topo;
    while (p != NULL) {
        EmailNode *prox = p->prox;
        p->prox = reverso;
        reverso = p;
        p = prox;
    }
    topo = reverso;

    fclose(arquivo);
    printf(COR_DADO "  Bandeja salva com sucesso em %s!\n" COR_RESET, ARQUIVO_CSV);
}

void liberarPilha(EmailNode **stack) {
    EmailNode *p = *stack;
    while (p != NULL) {
        EmailNode *prox = p->prox;
        free(p);
        p = prox;
    }
    *stack = NULL;
}

void carregarCSV() {
    FILE *arquivo = fopen(ARQUIVO_CSV, "r");
    if (arquivo == NULL) {
        return;
    }

    char campo[TAM_CORPO];
    int fimLinha;

    do {
        fimLinha = lerCampoCSV(arquivo, campo, sizeof(campo));
    } while (fimLinha == 0);

    if (fimLinha == -1) { fclose(arquivo); return; }

    liberarPilha(&topo);
    topo = NULL;

    int count = 0;
    while (1) {
        EmailNode *novo = (EmailNode *)malloc(sizeof(EmailNode));
        if (novo == NULL) {
            printf(COR_AVISO "  [ERRO] Memoria insuficiente!\n" COR_RESET);
            fclose(arquivo);
            return;
        }

        fimLinha = lerCampoCSV(arquivo, campo, sizeof(campo));
        if (campo[0] == '\0' && fimLinha == -1) { free(novo); break; }
        novo->id = atoi(campo);

        lerCampoCSV(arquivo, campo, sizeof(campo));
        copiar_campo_seguro(novo->remetente, campo, TAM_STR);

        lerCampoCSV(arquivo, campo, sizeof(campo));
        copiar_campo_seguro(novo->assunto, campo, TAM_STR);

        fimLinha = lerCampoCSV(arquivo, campo, sizeof(campo));
        copiar_campo_seguro(novo->data, campo, TAM_STR);

        novo->categoria[0] = '\0';
        novo->corpo[0] = '\0';

        novo->prox = topo;
        topo = novo;
        count++;

        if (novo->id >= proximo_id) proximo_id = novo->id + 1;

        if (fimLinha == -1) break;
    }

    fclose(arquivo);
    printf(COR_DADO "  Bandeja carregada de %s (%d e-mail(is) encontrado(s)).\n" COR_RESET, ARQUIVO_CSV, count);
}

void menu() {
    int opcao;

    do {
        limpar_tela();
        printf(COR_TITULO "  +================================+\n" COR_RESET);
        printf(COR_TITULO "  |       BANDEJA DE E-MAILS         |\n" COR_RESET);
        printf(COR_TITULO "  +================================+\n" COR_RESET);
        printf(COR_MENU    "  |  1. Ver bandeja                  |\n" COR_RESET);
        printf(COR_MENU    "  |  2. Ler proximo e-mail           |\n" COR_RESET);
        printf(COR_MENU    "  |  3. Ver e-mail no topo            |\n" COR_RESET);
        printf(COR_MENU    "  |  4. Marcar e-mail como lido      |\n" COR_RESET);
        printf(COR_MENU    "  |  5. Buscar e-mail                |\n" COR_RESET);
        printf(COR_MENU    "  |  6. Simular recebimento          |\n" COR_RESET);
        printf(COR_MENU    "  |  7. Excluir e-mail               |\n" COR_RESET);
        printf(COR_MENU    "  |  8. Salvar bandeja em CSV        |\n" COR_RESET);
        printf(COR_MENU    "  |  9. Carregar bandeja do CSV      |\n" COR_RESET);
        printf(COR_MENU    "  |  0. Sair                         |\n" COR_RESET);
        printf(COR_TITULO "  +================================+\n\n" COR_RESET);

        if (!ler_inteiro(COR_MENU "  Opcao: " COR_RESET, &opcao)) {
            pausar();
            continue;
        }

        switch (opcao) {
            case 1: exibir_bandeja();    break;
            case 2: ler_proximo_email(); break;
            case 3: peek_email();        break;
            case 4: {
                limpar_tela();
                int id;
                if (ler_inteiro(COR_MENU "  ID do e-mail a marcar como lido: " COR_RESET, &id))
                    marcar_como_lido(id);
                else
                    pausar();
                break;
            }
            case 5: buscar_email();         break;
            case 6: simular_recebimento();  break;
            case 7: {
                limpar_tela();
                int id;
                if (ler_inteiro(COR_MENU "  ID do e-mail a excluir: " COR_RESET, &id))
                    excluir_email(id);
                else
                    pausar();
                break;
            }
            case 8:
                limpar_tela();
                salvarCSV();
                pausar();
                break;
            case 9:
                limpar_tela();
                carregarCSV();
                pausar();
                break;
            case 0:
                limpar_tela();
                printf(COR_DADO "  Salvando bandeja antes de sair...\n" COR_RESET);
                salvarCSV();
                printf(COR_DADO "\n  Encerrando...\n\n" COR_RESET);
                break;
            default:
                printf(COR_AVISO "\n  Opcao invalida.\n" COR_RESET);
                pausar();
        }
    } while (opcao != 0);
}

int main() {
    carregarCSV();

    if (pilha_vazia()) {
        receber_email("alice@email.com",  "Reuniao amanha",           "24/06/2026 08:10", "Trabalho",
            "Oi, so para lembrar que temos reuniao amanha as 10h na sala 3. Por favor confirme presenca.");
        receber_email("bob@email.com",    "Proposta comercial",       "24/06/2026 09:30", "Negocios",
            "Segue em anexo nossa proposta atualizada. Aguardo seu retorno ate sexta-feira.");
    }

    menu();
    liberarPilha(&topo);
    liberarPilha(&topo_aux);
    return 0;
}
