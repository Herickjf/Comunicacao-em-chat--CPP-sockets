# Comunicação em Chats - CPP Sockets
Projeto Final da disciplina de Programação Concorrente e Distribuída, orientada pelo professor Carlos Eduardo Batista.

---

1. `v1-logging` - `25/09/25`
    Primeira entrega do projeto. Para executar, rode os seguintes comandos na pasta raiz:
    ```cmd
    make
    make run
    ```

<br>

<br>

2. `v2-cli` - `03/10/25`
    Etapa de criação dos lados de `server` e `cliente`, command line interface e scripts teste.
    - Para executar o projeto e testar por conta própria, execute:
        - Compilar:
            ```cmd
            make clear
            make
            ```
        - Executar o **server** side:
            ```cmd
            ./bin/server
            ```
        - Executar o **client** side (`n` clientes em `n` terminais distintos):
            ```cmd
            ./bin/client 127.0.0.1 8080 ClienteName
            ```
    <br>

    - Para rodar um script teste:
        ```cmd
        chmod +x test_script.sh
        ./test_script.sh
        ```

3. `v3-final` - `03/10/25`
Ajustes finais no `servidor`-`cliente`, criação de um [relatório](./relatorio.md) detalhado sobre o funcionamento do projeto e a aplciação das ideias propostas na v1, analisando se foi tudo concluído adequadamente, verificando o fluxo de funcionamento e explicitando o diagrama de sequência.