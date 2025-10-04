#include <iostream>
#include <thread>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include "libs/libtslog.hpp"

using namespace std;

class ChatClient {
private:
    int sock;
    struct sockaddr_in serv_addr;
    bool connected;
    string client_name;

public:
    ChatClient(const string& name) : connected(false), client_name(name) {}

    bool connectToServer(const string& ip, int port) {
        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0) {
            LOG_ERROR("Falha ao criar socket", "Network");
            return false;
        }

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(port);

        if (inet_pton(AF_INET, ip.c_str(), &serv_addr.sin_addr) <= 0) {
            LOG_ERROR("Endereço inválido ou não suportado: " + ip, "Network");
            return false;
        }

        if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            LOG_ERROR("Falha na conexão com o servidor " + ip + ":" + to_string(port), "Network");
            return false;
        }

        // Enviar nome para o servidor
        send(sock, client_name.c_str(), client_name.length(), 0);

        connected = true;
        LOG_SUCCESS("Conectado ao servidor " + ip + ":" + to_string(port), "Connection");
        return true;
    }

    void receiveMessages() {
        char buffer[1024] = {0};
        
        while (connected) {
            memset(buffer, 0, sizeof(buffer));
            int bytes_read = read(sock, buffer, sizeof(buffer) - 1);
            
            if (bytes_read <= 0) {
                LOG_ERROR("Conexão com servidor perdida", "Network");
                connected = false;
                break;
            }
            
            cout << buffer;
            cout.flush();
        }
    }

    void sendMessages() {
        string message;
        
        cout << "Conectado como: " << client_name << endl;
        cout << "Digite suas mensagens ou comandos:" << endl;
        
        while (connected) {
            cout << "> ";
            cout.flush();
            
            getline(cin, message);
            
            if (!connected) break;
            
            if (message == "/quit") {
                connected = false;
                send(sock, message.c_str(), message.length(), 0);
                break;
            }
            
            if (send(sock, message.c_str(), message.length(), 0) < 0) {
                LOG_ERROR("Falha ao enviar mensagem", "Network");
                connected = false;
                break;
            }
        }
    }

    void start() {
        if (!connected) {
            LOG_ERROR("Cliente não conectado ao servidor", "Connection");
            return;
        }

        LOG_INFO("Iniciando comunicação...", "ChatClient");

        // Thread para receber mensagens
        thread receiver(&ChatClient::receiveMessages, this);
        
        // Thread principal para enviar mensagens
        sendMessages();

        // Aguardar thread de recebimento
        if (receiver.joinable()) {
            receiver.join();
        }

        close(sock);
        LOG_INFO("Cliente desconectado", "ChatClient");
    }
};

int main(int argc, char* argv[]) {
    if (argc != 4) {
        cout << "Chat Client - Uso: " << argv[0] << " <IP> <PORTA> <NOME>" << endl;
        cout << "Exemplo: " << argv[0] << " 127.0.0.1 8080 João" << endl;
        return 1;
    }

    string ip = argv[1];
    int port = stoi(argv[2]);
    string name = argv[3];

    LOG_INFO("Iniciando cliente como: " + name, "ChatClient");
    LOG_INFO("Conectando ao servidor: " + ip + ":" + to_string(port), "Connection");

    ChatClient client(name);
    
    if (client.connectToServer(ip, port)) {
        client.start();
    } else {
        LOG_ERROR("Falha ao conectar com o servidor", "Connection");
        return 1;
    }

    return 0;
}