# Sprint 01 — fundação SLA nativa em C++

Objetivo: disponibilizar um fluxo SLA seguro e verificável no BambuStudio,
preservando o núcleo C++ e sem alegar compatibilidade com formatos ou máquinas
que ainda não foram validados.

## Escopo e ordem de execução

1. **Presets SLA persistentes — concluído nesta sprint**
   - Carregar e salvar processo e material SLA com a mesma chave de
     configuração.
   - Aceite: após reiniciar o aplicativo, o processo e o material SLA
     selecionados são restaurados.

2. **Rotas de exportação FFF e SLA — concluído nesta sprint**
   - Manter os comandos `Export plate sliced file` exclusivos de FFF.
   - Habilitar o exportador geral somente quando a fatia SLA estiver concluída.
   - Aceite: em SLA, apenas o exportador geral fica disponível e abre o fluxo
     de arquivo SLA; em FFF, os exportadores de placa permanecem inalterados.

3. **Perfil de referência seguro — concluído nesta sprint**
   - Tratar `GenericResin` como perfil de calibração para SL1 compatível, não
     como perfil universal MSLA.
   - Aceite: nome, descrição e mensagens da interface deixam explícito o
     formato suportado e a necessidade de calibração.

4. **Cobertura de testes de preset e exportação — em andamento**
   - Criar testes C++ para descoberta do bundle, seleção de presets e
     configuração SLA consolidada; adicionar um caso de exportação SL1 com
     artefato de referência quando o ambiente de build estiver disponível.
   - Aceite: testes executam no CI e cobrem sucesso e perfil incompatível.

5. **Validação de interface SLA — planejado**
   - Exercitar abas de processo/material, suporte, hollowing e pad em projeto
     SLA, incluindo troca FFF ↔ SLA.
   - Aceite: não há regressão visual ou ação FFF acessível no modo SLA.

6. **Exportação SLA por linha de comando — planejado**
   - Reativar o caminho CLI somente após compartilhar a mesma validação e o
     mesmo formato usado pela interface.
   - Aceite: execução headless gera um arquivo SLA válido e reporta falhas de
     configuração de maneira determinística.

7. **Formatos e recursos avançados — descoberta técnica**
   - Especificar exportadores CTB/GOO, perfis reais por impressora e melhorias
     de suporte, ilhas e hollowing em C++, com testes de arquivos dourados.
   - Aceite: cada formato terá especificação, casos de compatibilidade em
     hardware e testes independentes. Referências Rust são consulta de
     comportamento; código AGPL não será incorporado.

## Fora do escopo desta sprint

- Declarar suporte a qualquer impressora MSLA genérica.
- Portar ou incorporar código Rust de projetos externos.
- Habilitar CTB/GOO sem arquivos de referência e validação em hardware.
