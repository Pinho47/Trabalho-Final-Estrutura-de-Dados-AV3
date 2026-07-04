
/*
 * Trabalho A — Playlist Musical (Tema A1)
 * Lista encadeada com alocação dinâmica.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARQUIVO_CSV "Trabalho_A\\dados_a.csv"

/* Cores ANSI para destacar títulos, erros e dados no terminal */
#define COR_RESET   "\x1b[0m"
#define COR_TITULO  "\x1b[1;36m"
#define COR_OK      "\x1b[1;32m"
#define COR_ERRO    "\x1b[1;31m"
#define COR_AVISO   "\x1b[1;33m"
#define COR_MENU    "\x1b[1;37m"
#define COR_DADO    "\x1b[0;36m"

/* Nó da lista encadeada: cada música é um nó com ponteiro para o próximo */
typedef struct MusicaNode {
    int id;
    char titulo[100];
    char artista[100];
    int duracao;            /* armazenada em segundos (int), exibida em MM:SS */
    char genero[50];
    struct MusicaNode *prox; /* ponteiro para o próximo nó da lista */
} MusicaNode;

/* Cabeça da lista encadeada e contador de nós */
MusicaNode *inicio = NULL;
int qtdMusicas = 0;

/* Limpa o buffer de entrada após leitura com scanf */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/* Lê uma linha de texto do usuário, removendo o '\n' do final */
void lerTexto(char *destino, int tamanho, const char *rotulo) {
    printf(COR_MENU "%s: " COR_RESET, rotulo);
    fgets(destino, tamanho, stdin);
    destino[strcspn(destino, "\n")] = '\0';
}

/* Lê um número inteiro com validação — repete até receber entrada válida */
int lerInteiro(const char *rotulo) {
    int valor;
    int ok;
    do {
        printf(COR_MENU "%s: " COR_RESET, rotulo);
        ok = scanf("%d", &valor);
        limparBuffer();
        if (!ok) {
            printf(COR_ERRO "Entrada invalida. Digite um numero.\n" COR_RESET);
        }
    } while (!ok);
    return valor;
}

/* Busca um nó pelo ID, percorrendo a lista do início ao fim */
MusicaNode* buscarPorId(int id) {
    MusicaNode *atual = inicio;
    while (atual != NULL) {
        if (atual->id == id) return atual;
        atual = atual->prox;
    }
    return NULL;
}

/* Gera o próximo ID: percorre a lista, encontra o maior ID e soma 1 */
/* Gera próximo ID: maior ID da lista + 1 */
int gerarProximoId() {
    int maior = 0;
    MusicaNode *atual = inicio;
    while (atual != NULL) {
        if (atual->id > maior) maior = atual->id;
        atual = atual->prox;
    }
    return maior + 1;
}

/* Cadastra uma nova música: aloca nó com malloc, insere no final da lista */
void cadastrarMusica() {
    MusicaNode *novo = (MusicaNode*)malloc(sizeof(MusicaNode));
    if (novo == NULL) {
        printf(COR_ERRO "\nMemoria insuficiente! Nao e possivel cadastrar mais musicas.\n" COR_RESET);
        return;
    }

    novo->id = gerarProximoId();
    novo->prox = NULL;

    printf(COR_TITULO "\n--- Cadastro de Musica ---\n" COR_RESET);
    printf(COR_AVISO "ID gerado automaticamente: %d\n" COR_RESET, novo->id);

    lerTexto(novo->titulo, sizeof(novo->titulo), "Titulo");
    if (strlen(novo->titulo) == 0) {
        printf(COR_ERRO "Titulo nao pode ser vazio. Cadastro cancelado.\n" COR_RESET);
        free(novo); /* libera o nó alocado se cadastro for cancelado */
        return;
    }

    lerTexto(novo->artista, sizeof(novo->artista), "Artista");
    if (strlen(novo->artista) == 0) {
        printf(COR_ERRO "Artista nao pode ser vazio. Cadastro cancelado.\n" COR_RESET);
        free(novo);
        return;
    }

    novo->duracao = lerInteiro("Duracao em segundos");
    if (novo->duracao <= 0) {
        printf(COR_ERRO "Duracao invalida. Cadastro cancelado.\n" COR_RESET);
        free(novo);
        return;
    }

    lerTexto(novo->genero, sizeof(novo->genero), "Genero");
    if (strlen(novo->genero) == 0) {
        printf(COR_ERRO "Genero nao pode ser vazio. Cadastro cancelado.\n" COR_RESET);
        free(novo);
        return;
    }

    /* Insere no final da lista: percorre até o último nó */
    if (inicio == NULL) {
        inicio = novo;
    } else {
        MusicaNode *atual = inicio;
        while (atual->prox != NULL) atual = atual->prox;
        atual->prox = novo;
    }
    qtdMusicas++;

    printf(COR_OK "\nMusica cadastrada com sucesso!\n" COR_RESET);
}

/* Lista todas as músicas da playlist com ID, título, artista, duração MM:SS e gênero */
void listarMusicas() {
    printf(COR_TITULO "\n--- Playlist (%d musica(s)) ---\n" COR_RESET, qtdMusicas);

    if (qtdMusicas == 0) {
        printf(COR_AVISO "A playlist esta vazia no momento.\n" COR_RESET);
        return;
    }

    printf(COR_MENU "%-5s %-25s %-25s %-10s %-15s\n" COR_RESET,
           "ID", "Titulo", "Artista", "Duracao", "Genero");
    printf("-----------------------------------------------------------------------------\n");

    MusicaNode *atual = inicio;
    while (atual != NULL) {
        /* Converte segundos para MM:SS na exibição */
        printf(COR_DADO "%-5d %-25s %-25s %02d:%02d     %-15s\n" COR_RESET,
               atual->id,
               atual->titulo,
               atual->artista,
               atual->duracao / 60,
               atual->duracao % 60,
               atual->genero);
        atual = atual->prox;
    }
}

/* Busca uma música pelo ID e exibe seus dados completos */
void buscarMusica() {
    if (qtdMusicas == 0) {
        printf(COR_AVISO "\nA playlist esta vazia. Nada para buscar.\n" COR_RESET);
        return;
    }

    int id = lerInteiro("Digite o ID da musica que deseja buscar");
    MusicaNode *no = buscarPorId(id);

    if (no == NULL) {
        printf(COR_ERRO "\nNenhuma musica encontrada com o ID %d.\n" COR_RESET, id);
        return;
    }

    printf(COR_TITULO "\n--- Musica Encontrada ---\n" COR_RESET);
    printf(COR_DADO "ID: %d\nTitulo: %s\nArtista: %s\nDuracao: %02d:%02d\nGenero: %s\n" COR_RESET,
           no->id,
           no->titulo,
           no->artista,
           no->duracao / 60,
           no->duracao % 60,
           no->genero);
}

/* Edita uma música: campos em branco mantêm o valor atual */
void editarMusica() {
    if (qtdMusicas == 0) {
        printf(COR_AVISO "\nA playlist esta vazia. Nada para editar.\n" COR_RESET);
        return;
    }

    int id = lerInteiro("Digite o ID da musica que deseja editar");
    MusicaNode *no = buscarPorId(id);

    if (no == NULL) {
        printf(COR_ERRO "\nNenhuma musica encontrada com o ID %d.\n" COR_RESET, id);
        return;
    }

    printf(COR_AVISO "\nDeixe em branco e pressione Enter para manter o valor atual.\n" COR_RESET);

    char buffer[100];

    printf(COR_DADO "Titulo atual: %s\n" COR_RESET, no->titulo);
    lerTexto(buffer, sizeof(buffer), "Novo titulo");
    if (strlen(buffer) > 0) strcpy(no->titulo, buffer);

    printf(COR_DADO "Artista atual: %s\n" COR_RESET, no->artista);
    lerTexto(buffer, sizeof(buffer), "Novo artista");
    if (strlen(buffer) > 0) strcpy(no->artista, buffer);

    printf(COR_DADO "Duracao atual: %02d:%02d\n" COR_RESET, no->duracao / 60, no->duracao % 60);
    printf(COR_MENU "Nova duracao (0 para manter): " COR_RESET);
    int novaDuracao;
    if (scanf("%d", &novaDuracao) == 1 && novaDuracao > 0) {
        no->duracao = novaDuracao;
    }
    limparBuffer();

    printf(COR_DADO "Genero atual: %s\n" COR_RESET, no->genero);
    lerTexto(buffer, sizeof(buffer), "Novo genero");
    if (strlen(buffer) > 0) strcpy(no->genero, buffer);

    printf(COR_OK "\nMusica atualizada com sucesso!\n" COR_RESET);
}

/* Exclui uma música com confirmação, ajusta ponteiros e libera memória */
void excluirMusica() {
    if (qtdMusicas == 0) {
        printf(COR_AVISO "\nA playlist esta vazia. Nada para excluir.\n" COR_RESET);
        return;
    }

    int id = lerInteiro("Digite o ID da musica que deseja excluir");

    /* Percorre a lista mantendo referência ao nó anterior */
    MusicaNode *atual = inicio;
    MusicaNode *anterior = NULL;

    while (atual != NULL && atual->id != id) {
        anterior = atual;
        atual = atual->prox;
    }

    if (atual == NULL) {
        printf(COR_ERRO "\nNenhuma musica encontrada com o ID %d.\n" COR_RESET, id);
        return;
    }

    printf(COR_AVISO "Tem certeza que deseja excluir \"%s\"? (S/N): " COR_RESET, atual->titulo);
    char confirmacao;
    scanf(" %c", &confirmacao);
    limparBuffer();

    if (confirmacao != 'S' && confirmacao != 's') {
        printf(COR_AVISO "Exclusao cancelada.\n" COR_RESET);
        return;
    }

    /* Desencadeia o nó: atualiza ponteiro do anterior ou da cabeça */
    if (anterior == NULL) {
        inicio = atual->prox; /* remoção do primeiro nó */
    } else {
        anterior->prox = atual->prox; /* remoção do meio ou fim */
    }
    free(atual); /* libera a memória alocada para o nó */
    qtdMusicas--;

    printf(COR_OK "\nMusica excluida com sucesso!\n" COR_RESET);
}

/* Exibe estatísticas: duração total, música mais longa, contagem por gênero */
void exibirEstatisticas() {
    printf(COR_TITULO "\n--- Estatisticas da Playlist ---\n" COR_RESET);

    if (qtdMusicas == 0) {
        printf(COR_AVISO "A playlist esta vazia. Nenhuma estatistica disponivel.\n" COR_RESET);
        return;
    }

    int totalSegundos = 0;
    int segMaisLonga = 0;
    MusicaNode *maisLonga = inicio;

    /* Arrays temporários para contar músicas por gênero */
    char generos[qtdMusicas][50];
    int contagem[qtdMusicas];
    int totalGeneros = 0;

    MusicaNode *atual = inicio;
    while (atual != NULL) {
        totalSegundos += atual->duracao;

        /* Rastreia a música com maior duração */
        if (atual->duracao > segMaisLonga) {
            segMaisLonga = atual->duracao;
            maisLonga = atual;
        }

        /* Verifica se o gênero já foi registrado */
        int encontrado = -1;
        for (int j = 0; j < totalGeneros; j++) {
            if (strcmp(generos[j], atual->genero) == 0) {
                encontrado = j;
                break;
            }
        }
        if (encontrado == -1) {
            strcpy(generos[totalGeneros], atual->genero);
            contagem[totalGeneros] = 1;
            totalGeneros++;
        } else {
            contagem[encontrado]++;
        }

        atual = atual->prox;
    }

    printf(COR_DADO "Total de musicas: %d\n" COR_RESET, qtdMusicas);
    printf(COR_DADO "Duracao total da playlist: %02d:%02d\n" COR_RESET, totalSegundos / 60, totalSegundos % 60);
    printf(COR_DADO "Musica mais longa: %s (%02d:%02d)\n" COR_RESET, maisLonga->titulo, maisLonga->duracao / 60, maisLonga->duracao % 60);
    printf(COR_DADO "Generos cadastrados:\n" COR_RESET);
    for (int i = 0; i < totalGeneros; i++) {
        printf(COR_DADO "  - %s: %d musica(s)\n" COR_RESET, generos[i], contagem[i]);
    }
}

/* RFC 4180: escreve um campo CSV, envolvendo em aspas se contiver ; " ou \n */
/* Escrita CSV: envolve campo entre aspas se contiver caracteres especiais */
void escreverCampoCSV(FILE *arquivo, const char *campo) {
    int precisaAspas = (strchr(campo, ';') != NULL ||
                         strchr(campo, '"')  != NULL ||
                         strchr(campo, '\n') != NULL);

    if (precisaAspas) {
        fputc('"', arquivo);
        for (const char *p = campo; *p != '\0'; p++) {
            if (*p == '"') fputc('"', arquivo); /* duplica aspas internas */
            fputc(*p, arquivo);
        }
        fputc('"', arquivo);
    } else {
        fputs(campo, arquivo);
    }
}

/* RFC 4180: lê um campo CSV caractere a caractere, tratando aspas e delimitadores
 * Retorna 1 se encontrou fim de linha, 0 se campo intermediário, -1 se EOF */
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
                    if (i < tamanho - 1) destino[i++] = '"'; /* aspas duplas escapadas */
                } else {
                    ungetc(prox, arquivo);
                    comAspas = 0; /* fim das aspas */
                }
            } else if (i < tamanho - 1) {
                destino[i++] = (char)c;
            }
        } else {
            if (c == '\r') continue; /* ignora carriage return */
            if (c == ';' || c == '\n') { destino[i] = '\0'; return (c == '\n'); }
            if (i < tamanho - 1) destino[i++] = (char)c;
        }
    }
}

/* Cópia segura de string: garante terminação \0 mesmo se o campo for maior */
void copiarCampoSeguro(char *destino, const char *origem, size_t tamanhoDestino) {
    strncpy(destino, origem, tamanhoDestino - 1);
    destino[tamanhoDestino - 1] = '\0';
}

/* Salva toda a lista encadeada em CSV, percorrendo nó a nó */
void salvarCSV() {
    FILE *arquivo = fopen(ARQUIVO_CSV, "w");
    if (arquivo == NULL) {
        printf(COR_ERRO "\nErro ao abrir o arquivo para salvar.\n" COR_RESET);
        return;
    }

    fprintf(arquivo, "id;titulo;artista;duracao;genero\n");
    MusicaNode *atual = inicio;
    while (atual != NULL) {
        fprintf(arquivo, "%d;", atual->id);
        escreverCampoCSV(arquivo, atual->titulo);  fputc(';', arquivo);
        escreverCampoCSV(arquivo, atual->artista); fputc(';', arquivo);
        fprintf(arquivo, "%d;", atual->duracao);
        escreverCampoCSV(arquivo, atual->genero);
        fputc('\n', arquivo);
        atual = atual->prox;
    }

    fclose(arquivo);
    printf(COR_OK "\nPlaylist salva com sucesso em %s!\n" COR_RESET, ARQUIVO_CSV);
}

/* Libera toda a memória alocada pela lista encadeada */
void liberarPlaylist() {
    MusicaNode *atual = inicio;
    while (atual != NULL) {
        MusicaNode *temp = atual;
        atual = atual->prox;
        free(temp);
    }
    inicio = NULL;
}

/* Carrega músicas do CSV para a lista encadeada, recriando os nós com malloc */
void carregarCSV() {
    liberarPlaylist();
    qtdMusicas = 0;

    FILE *arquivo = fopen(ARQUIVO_CSV, "r");
    if (arquivo == NULL) {
        return; /* arquivo pode não existir na primeira execução */
    }

    char campo[300];
    int fimLinha;

    /* Pula o cabeçalho (primeira linha) */
    do {
        fimLinha = lerCampoCSV(arquivo, campo, sizeof(campo));
    } while (fimLinha == 0);

    if (fimLinha == -1) { fclose(arquivo); return; }

    /* Lê cada linha do CSV e reconstitui os nós da lista */
    while (1) {
        MusicaNode *novo = (MusicaNode*)malloc(sizeof(MusicaNode));
        if (novo == NULL) {
            printf(COR_ERRO "Erro de memoria ao carregar playlist.\n" COR_RESET);
            break;
        }
        novo->prox = NULL;

        fimLinha = lerCampoCSV(arquivo, campo, sizeof(campo));
        if (campo[0] == '\0' && fimLinha == -1) { free(novo); break; }
        novo->id = atoi(campo);

        if (lerCampoCSV(arquivo, campo, sizeof(campo)) == -1 && campo[0] == '\0') { free(novo); break; }
        copiarCampoSeguro(novo->titulo, campo, sizeof(novo->titulo));

        if (lerCampoCSV(arquivo, campo, sizeof(campo)) == -1 && campo[0] == '\0') { free(novo); break; }
        copiarCampoSeguro(novo->artista, campo, sizeof(novo->artista));

        if (lerCampoCSV(arquivo, campo, sizeof(campo)) == -1 && campo[0] == '\0') { free(novo); break; }
        novo->duracao = atoi(campo);

        fimLinha = lerCampoCSV(arquivo, campo, sizeof(campo));
        copiarCampoSeguro(novo->genero, campo, sizeof(novo->genero));

        /* Insere no final da lista */
        if (inicio == NULL) {
            inicio = novo;
        } else {
            MusicaNode *ultimo = inicio;
            while (ultimo->prox != NULL) ultimo = ultimo->prox;
            ultimo->prox = novo;
        }
        qtdMusicas++;

        if (fimLinha == -1) break;
    }

    fclose(arquivo);
    printf(COR_OK "Playlist carregada de %s (%d musica(s) encontrada(s)).\n" COR_RESET, ARQUIVO_CSV, qtdMusicas);
}

/* Exibe o menu principal com todas as opções */
void exibirMenu() {
    printf(COR_TITULO "\n=====================================\n" COR_RESET);
    printf(COR_TITULO "        MINHA PLAYLIST MUSICAL\n" COR_RESET);
    printf(COR_TITULO "=====================================\n" COR_RESET);
    printf(COR_MENU "1 - Cadastrar musica\n" COR_RESET);
    printf(COR_MENU "2 - Listar musicas\n" COR_RESET);
    printf(COR_MENU "3 - Buscar musica por ID\n" COR_RESET);
    printf(COR_MENU "4 - Editar musica\n" COR_RESET);
    printf(COR_MENU "5 - Excluir musica\n" COR_RESET);
    printf(COR_MENU "6 - Exibir estatisticas\n" COR_RESET);
    printf(COR_MENU "7 - Salvar playlist em CSV\n" COR_RESET);
    printf(COR_MENU "8 - Carregar playlist do CSV\n" COR_RESET);
    printf(COR_MENU "0 - Sair\n" COR_RESET);
    printf(COR_TITULO "=====================================\n" COR_RESET);
}

/* Função principal: carrega CSV ao iniciar, loop do menu, salva e libera ao sair */
int main() {
    carregarCSV(); /* carrega dados persistidos na execução anterior */

    int opcao;
    do {
        exibirMenu();
        opcao = lerInteiro("Escolha uma opcao");

        switch (opcao) {
            case 1:
                cadastrarMusica();
                break;
            case 2:
                listarMusicas();
                break;
            case 3:
                buscarMusica();
                break;
            case 4:
                editarMusica();
                break;
            case 5:
                excluirMusica();
                break;
            case 6:
                exibirEstatisticas();
                break;
            case 7:
                salvarCSV();
                break;
            case 8:
                carregarCSV();
                break;
            case 0:
                printf(COR_OK "\nSalvando playlist antes de sair...\n" COR_RESET);
                salvarCSV();
                printf(COR_OK "Ate a proxima! Obrigado por usar a Minha Playlist Musical.\n" COR_RESET);
                break;
            default:
                printf(COR_ERRO "\nOpcao invalida. Tente novamente.\n" COR_RESET);
        }
    } while (opcao != 0);

    liberarPlaylist(); /* libera toda a memória alocada */
    return 0;
}
