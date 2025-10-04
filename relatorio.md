# Relatório de Análise com IA

## Visão Geral
O sistema implementa um modelo de **comunicação em chats com sockets em C++**, estruturado sobre a arquitetura clássica **cliente-servidor**.  
O objetivo é permitir que múltiplos clientes se conectem a um servidor central, ingressem em grupos de conversa pré-definidos e troquem mensagens em tempo real, com **gerenciamento de concorrência** e **controle de logs integrados**.

---

## Arquitetura Cliente-Servidor
- **Servidor**
  - Gerencia conexões TCP/IP.
  - Mantém lista de clientes ativos.
  - Controla 5 grupos fixos (*Geral, Desenvolvimento, Suporte, Vendas e Marketing*).
  - Encaminha mensagens apenas para membros do mesmo grupo.

- **Cliente**
  - Conecta-se ao servidor via socket.
  - Envia nome de usuário e escolhe grupo.
  - Utiliza **duas threads**:
    -  Envio de mensagens (interativo).
    -  Recebimento de mensagens (assíncrono, concorrente).

- **Grupos de Chat**
  - Armazenam os participantes conectados.
  - Mantêm contador de mensagens trocadas.
  - Gerenciam entradas, saídas e desconexões.

- **Sistema de Logs**
  - Implementado com a biblioteca **libtslog**.
  - Utiliza **TicketLock** para garantir acesso thread-safe.
  - Suporte a múltiplos níveis de severidade:
    - INFO, WARN, DEBUG, SUCCESS, ERROR.
  - Saída colorida e organizada, facilitando análise e depuração.

---

##  Fluxo de Funcionamento
1. Cliente estabelece conexão TCP com o servidor.  
2. Envia nome de usuário.  
3. Recebe a lista de grupos disponíveis.  
4. Escolhe um grupo para ingressar.  
5. O servidor associa o cliente ao grupo e envia mensagem de boas-vindas.  
6. A partir daí:
   - Todas as mensagens enviadas pelo cliente são redirecionadas para os demais membros do grupo.
   - O cliente também recebe mensagens enviadas por outros usuários do mesmo grupo.
7. Em caso de desconexão:
   - O servidor notifica os outros clientes.
   - A lista de participantes é atualizada.

---

##  Diagrama de Sequência (ASCII)

```text
Cliente                   Servidor                   Grupo
   |                          |                        |
   | ---- Conexão TCP ------->|                        |
   | ---- Envia Nome -------->|                        |
   |                          |--- Lista Grupos ------>|
   |<--- Exibe opções --------|                        |
   |---- Escolhe Grupo ------>|                        |
   |                          |--- addClient() ------->|
   |<--- Boas-vindas ---------|                        |
   |---- Mensagem ------------|------ Broadcast ------>|
   |                          |<-- Mensagem Grupo -----|
   |<--- Recebe Respostas ----|                        |
   |---- Desconexão ----------|                        |
   |                          |--- removeClient() ---->|
   |<--- Notificação ---------|                        |
```

##  Conclusão

O sistema caracteriza-se como uma aplicação de chat concorrente distribuído, com controle centralizado de fluxo, uso intensivo de threads e bloqueios de sincronização para consistência.
Demonstra, de forma prática, os principais conceitos de programação concorrente e distribuída:
- Gerenciamento de múltiplas conexões em paralelo.
- Coordenação de mensagens em grupos distintos.
- Registro confiável e thread-safe de logs de execução.