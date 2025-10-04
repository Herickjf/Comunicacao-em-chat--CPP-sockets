#include <iostream>
#include <thread>
#include <vector>
#include <memory>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <mutex>
#include <algorithm>
#include <csignal>
#include "libs/libtslog.hpp"
#include "libs/client_info.hpp"
#include "libs/chat_group.hpp"

using namespace std;

class ChatServer {
private:
    int server_fd;
    struct sockaddr_in address;
    vector<shared_ptr<ClientInfo>> all_clients;
    vector<shared_ptr<ChatGroup>> groups;
    mutex clients_mutex;
    bool running;
    const int MAX_GROUPS = 5;

public:
    ChatServer(int port) : running(false) {
        // Criar socket
        server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == 0) {
            LOG_ERROR("Falha ao criar socket");
            exit(EXIT_FAILURE);
        }

        // Configurar socket
        int opt = 1;
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
            LOG_ERROR("Falha ao configurar socket");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        // Bind
        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
            LOG_ERROR("Falha no bind");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Listen
        if (listen(server_fd, 10) < 0) {
            LOG_ERROR("Falha no listen");
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Inicializar grupos
        initializeGroups();
        
        LOG_SUCCESS("Servidor iniciado na porta " + to_string(port));
    }

private:
    void initializeGroups() {
        vector<string> group_names = {
            "Geral", "Desenvolvimento", "Suporte", "Vendas", "Marketing"
        };
        
        for (int i = 0; i < MAX_GROUPS; i++) {
            groups.push_back(make_shared<ChatGroup>(i + 1, group_names[i]));
            LOG_INFO("Grupo criado: " + groups.back()->getName(), "GroupManager");
        }
    }

    string getGroupsList() {
        string list = "\n" + string(50, '=') + "\n";
        list += "           GRUPOS DISPONÍVEIS\n";
        list += string(50, '=') + "\n";
        
        for (const auto& group : groups) {
            list += "[" + to_string(group->getId()) + "] " + group->getGroupInfo() + "\n";
        }
        
        list += string(50, '=') + "\n";
        list += "Comandos disponíveis:\n";
        list += "  /list - Ver grupos novamente\n";
        list += "  /quit - Sair do servidor\n";
        list += string(50, '=') + "\n";
        list += "Digite o número do grupo que deseja entrar (1-" + to_string(MAX_GROUPS) + "): ";
        return list;
    }

    void sendToClient(int client_fd, const string& message) {
        if (send(client_fd, message.c_str(), message.length(), 0) < 0) {
            LOG_ERROR("Falha ao enviar mensagem para cliente", "Network");
        }
    }

    void handleClient(int client_fd) {
        char buffer[1024] = {0};
        sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        
        getpeername(client_fd, (struct sockaddr*)&client_addr, &addr_len);
        string client_ip = inet_ntoa(client_addr.sin_addr);

        // Receber nome do cliente
        string client_name = "Cliente_" + client_ip;
        int bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
        if (bytes_read > 0) {
            buffer[bytes_read] = '\0';
            client_name = string(buffer);
            client_name.erase(client_name.find_last_not_of("\n\r") + 1);
        }

        // Criar info do cliente
        auto client_info = make_shared<ClientInfo>(client_fd, client_name, client_ip);
        
        {
            lock_guard<mutex> lock(clients_mutex);
            all_clients.push_back(client_info);
        }

        LOG_SUCCESS("Novo cliente conectado: " + client_name + " (" + client_ip + ")", "Connection");

        bool in_group = false;
        shared_ptr<ChatGroup> selected_group;

        while (!in_group && running) {
            // Enviar lista de grupos
            sendToClient(client_fd, getGroupsList());

            // Receber escolha do grupo
            memset(buffer, 0, sizeof(buffer));
            bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
            
            if (bytes_read <= 0) {
                removeClient(client_fd);
                return;
            }

            string input(buffer);
            input.erase(input.find_last_not_of("\n\r") + 1);

            if (input == "/list") {
                continue;
            }
            else if (input == "/quit") {
                removeClient(client_fd);
                return;
            }

            try {
                int group_choice = stoi(input);
                
                if (group_choice >= 1 && group_choice <= MAX_GROUPS) {
                    selected_group = groups[group_choice - 1];
                    in_group = true;
                } else {
                    sendToClient(client_fd, "\nGrupo inválido! Escolha um número entre 1 e " + to_string(MAX_GROUPS) + "\n\n");
                }
            } catch (const exception& e) {
                sendToClient(client_fd, "\nEntrada inválida! Digite um número ou comando.\n\n");
            }
        }

        if (!running) return;

        // Adicionar cliente ao grupo
        selected_group->addClient(client_info);
        
        string welcome_msg = "\n[SERVIDOR] " + client_name + " entrou no grupo '" + selected_group->getName() + "'\n";
        selected_group->broadcastMessage(welcome_msg, client_fd);
        
        string group_info = "\n" + string(40, '=') + "\n";
        group_info += "       GRUPO: " + selected_group->getName() + "\n";
        group_info += string(40, '=') + "\n";
        group_info += "👥 Clientes conectados (" + to_string(selected_group->getClientCount()) + "):\n";
        
        auto client_list = selected_group->getClientList();
        for (const auto& client_str : client_list) {
            group_info += "  • " + client_str + "\n";
        }
        
        group_info += string(40, '=') + "\n";
        group_info += "Comandos disponíveis:\n";
        group_info += "  /info - Ver informações do grupo\n";
        group_info += "  /list - Ver outros grupos\n"; 
        group_info += "  /quit - Sair do grupo\n";
        group_info += string(40, '=') + "\n";
        group_info += "Digite sua mensagem:\n";
        group_info += "> ";
        
        sendToClient(client_fd, group_info);

        LOG_SUCCESS(client_name + " entrou no grupo: " + selected_group->getName(), "GroupManager");

        // Loop de mensagens
        while (running) {
            memset(buffer, 0, sizeof(buffer));
            bytes_read = read(client_fd, buffer, sizeof(buffer) - 1);
            
            if (bytes_read <= 0) {
                break;
            }

            string message(buffer);
            message.erase(message.find_last_not_of("\n\r") + 1);

            if (message == "/quit") {
                break;
            } else if (message == "/info") {
                string info = "\n" + selected_group->getDetailedInfo() + "\n> ";
                sendToClient(client_fd, info);
            } else if (message == "/list") {
                string groups_info = "\nGrupos disponíveis:\n";
                for (const auto& group : groups) {
                    groups_info += "  " + group->getGroupInfo() + "\n";
                }
                groups_info += "\nVocê está no grupo: " + selected_group->getName() + "\n";
                groups_info += "Use /quit para sair deste grupo e entrar em outro.\n> ";
                sendToClient(client_fd, groups_info);
            } else if (!message.empty()) {
                string formatted_msg = "[" + client_name + "]: " + message + "\n";
                LOG_INFO("Grupo '" + selected_group->getName() + "' - " + client_name + ": " + message, "Chat");
                selected_group->broadcastMessage(formatted_msg, client_fd);
            }
        }

        removeClient(client_fd, selected_group, client_name);
    }

    void removeClient(int client_fd, shared_ptr<ChatGroup> group = nullptr, const string& name = "") {
        if (group) {
            string leave_msg = "[SERVIDOR] " + name + " saiu do grupo\n";
            group->broadcastMessage(leave_msg);
            group->removeClient(client_fd);
            LOG_WARN(name + " saiu do grupo: " + group->getName(), "GroupManager");
        }

        {
            lock_guard<mutex> lock(clients_mutex);
            all_clients.erase(
                remove_if(all_clients.begin(), all_clients.end(),
                    [client_fd](const shared_ptr<ClientInfo>& client) {
                        return client->socket_fd == client_fd;
                    }),
                all_clients.end()
            );
        }

        close(client_fd);
        LOG_INFO("Cliente desconectado: " + name, "Connection");
    }

public:
    void start() {
        running = true;
        LOG_INFO("Servidor aguardando conexões...", "Server");
        LOG_INFO("Total de grupos ativos: " + to_string(groups.size()), "GroupManager");

        while (running) {
            int client_fd;
            struct sockaddr_in client_addr;
            socklen_t client_addr_len = sizeof(client_addr);

            client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
            
            if (client_fd < 0) {
                if (running) {
                    LOG_ERROR("Falha ao aceitar conexão", "Network");
                }
                continue;
            }

            thread client_thread(&ChatServer::handleClient, this, client_fd);
            client_thread.detach();
        }
    }

    void stop() {
        running = false;
        close(server_fd);
        
        lock_guard<mutex> lock(clients_mutex);
        for (const auto& client : all_clients) {
            close(client->socket_fd);
        }
        all_clients.clear();
        
        LOG_INFO("Servidor parado", "Server");
    }

    ~ChatServer() {
        stop();
    }
};

int main() {
    // Capturar Ctrl+C para parada graciosa
    signal(SIGINT, [](int) {
        LOG_WARN("Recebido sinal de interrupção, parando servidor...", "Server");
        exit(0);
    });

    ChatServer server(8080);
    server.start();
    return 0;
}