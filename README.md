# Trabalho Final - Estruturas de Dados

## Integrantes
- Aquiles Angelo
- Caio Cunha Passinho
- Emmanuel de Pinho de Jesus
- Maria Rita

- Turma: C | CIÊNCIAS DA COMPUTAÇÃO

## Temas Escolhidos

### Trabalho A — Lista Simples
**Tema A1 — Playlist de Músicas**
Sistema de playlist musical utilizando lista simplesmente encadeada com alocação dinâmica.
Permite cadastrar, listar, buscar, editar, excluir músicas, exibir estatísticas (duração total, música mais longa, contagem por gênero), além de salvar e carregar dados em CSV seguindo o padrão RFC 4180.

### Trabalho B — Pilha Simples
**Tema B3 — Bandeja de E-mails Não Lidos**
Simulação de bandeja de e-mails utilizando pilha encadeada com alocação dinâmica (LIFO).
Permite receber, ler (desempilhar), buscar por ID ou trecho do remetente, marcar como lido (usando pilha auxiliar), excluir e-mails com confirmação, além de salvar e carregar dados em CSV com suporte a campos especiais.

### Trabalho C — Fila Simples
**Tema C1 — Atendimento Bancário**
Simulação de sistema de atendimento bancário com fila normal (linear encadeada) e fila preferencial (circular encadeada), ambas com alocação dinâmica.
Regra de atendimento: 2 senhas normais para cada 1 preferencial. Permite gerar senhas, atender, buscar, listar, cancelar em qualquer posição, relatório estatístico, salvar e carregar CSV.

## Instruções de Instalação e Execução

## Método mais simples:

- Executar os .exe já compilados pela nossa equipe pelo gcc, o sistema ja abre com o terminal funcionando.

## Método mais complicado:

### Pré-requisitos
- Compilador GCC instalado no computador (ex: MinGW no Windows)
- **Não é possível compilar e executar em compiladores online** (como OnlineGDB, Programiz, etc.), pois os programas realizam leitura e escrita de arquivos CSV no disco local. Compiladores online não possuem acesso ao sistema de arquivos local, portanto as funções de persistência não irão funcionar.

### Compilar (usando GCC)


Abra o terminal (PowerShell ou CMD) na pasta do projeto e execute:

```bash
gcc -o trabalho_a.exe Trabalho_A/trabalho_a.c
gcc -o trabalho_b.exe Trabalho_B/trabalho_b.c
gcc -o trabalho_c.exe Trabalho_C/trabalho_c.c
```

### Executar via terminal

```bash
.\trabalho_a.exe
.\trabalho_b.exe
.\trabalho_c.exe
```

### Executar via arquivo .exe

Após compilar, os arquivos `.exe` serão gerados na pasta raiz do projeto. Basta fazer duplo clique em cada um ou executar pelo terminal conforme o comando acima.

## Observações

...