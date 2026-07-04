# Relatório do Trabalho Final

## Dificuldades Encontradas

1. **Marcar e-mail como lido**: A pilha não permite acesso direto ao meio. A solução foi usar uma pilha auxiliar para desempilhar até encontrar o e-mail certo e depois devolver tudo à pilha original.

2. **Fila circular preferencial**: Lógica diferente da fila normal, com cuidado especial para os casos de fila vazia e remoção de elemento.

3. **Gravação do vídeo**: Foi difícil compactar todo o conteúdo dos três sistemas em no máximo 10 minutos. Caso algum detalhe tenha ficado de fora, solicitamos encarecidamente que seja relevado na avaliação.

## Funcionalidades Extras Implementadas

### Trabalho A — Playlist Musical
- Estatísticas: duração total da playlist, música mais longa, contagem de músicas por gênero
- Confirmação antes de excluir música
- Edição flexível: campos em branco mantêm o valor atual
- Geração automática e sequencial de ID
- Cores ANSI no terminal
- Persistência CSV com padrão RFC 4180 e cópia segura de strings
- Lista simplesmente encadeada com alocação dinâmica (malloc/free)

### Trabalho B — E-mails
- Peek: ver e-mail no topo sem removê-lo
- Busca por ID ou por trecho do remetente
- Marcação de e-mail como lido usando pilha auxiliar
- Confirmação antes de excluir e-mail
- Validação de data (DD/MM/AAAA HH:MM)
- Cores ANSI no terminal
- Persistência CSV com padrão RFC 4180
- Pilha encadeada com alocação dinâmica (malloc/free)

### Trabalho C — Atendimento Bancário
- Regra de prioridade 2 normais para 1 preferencial
- Consultar próxima senha sem atender (peek)
- Cancelamento de senha em qualquer posição da fila
- Relatório completo de estatísticas de atendimento
- Horário de chegada automático
- Fila preferencial circular encadeada
- Busca por tipo e número de senha
- Fila normal linear encadeada e fila preferencial circular encadeada, ambas com alocação dinâmica (malloc/free)

## Referências

- KERNIGHAN, B. W.; RITCHIE, D. M. **The C Programming Language**. 2. ed. Prentice Hall, 1988.
- **Enunciado do Trabalho — Prof. Nirton Afonso**. Disponível em: https://github.com/NirtonAfonso/Trabalho-Estruturas-de-Dados-2026
- **Remotion — Documentação oficial**. Disponível em: https://www.remotion.dev/docs
- **RFC 4180 — CSV Format**. Disponível em: https://datatracker.ietf.org/doc/html/rfc4180
- **cppreference — C Standard Library**. Disponível em: https://en.cppreference.com/w/c
- **ANSI escape codes**. Disponível em: https://en.wikipedia.org/wiki/ANSI_escape_code

## Observações

...
