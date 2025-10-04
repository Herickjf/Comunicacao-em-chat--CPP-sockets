#ifndef CHAT_GROUP_HPP
#define CHAT_GROUP_HPP

#include <vector>
#include <string>
#include <memory>
#include <mutex>
#include <algorithm>
#include "client_info.hpp"
#include "libtslog.hpp"

using namespace std;

class ChatGroup {
private:
    int group_id;
    string name;
    vector<shared_ptr<ClientInfo>> clients;
    mutable mutex clients_mutex; // mutable para poder usar em funções const
    int message_count;

public:
    ChatGroup(int id, const string& group_name) 
        : group_id(id), name(group_name), message_count(0) {}

    int getId() const { return group_id; }
    string getName() const { return name; }
    
    int getClientCount() const { 
        lock_guard<mutex> lock(clients_mutex);
        return clients.size(); 
    }
    
    int getMessageCount() const { return message_count; }

    void addClient(shared_ptr<ClientInfo> client) {
        lock_guard<mutex> lock(clients_mutex);
        clients.push_back(client);
        client->current_group_id = group_id;
        LOG_INFO(client->name + " adicionado ao grupo", "Group-" + name);
    }

    void removeClient(int client_socket) {
        lock_guard<mutex> lock(clients_mutex);
        auto it = find_if(clients.begin(), clients.end(),
            [client_socket](const shared_ptr<ClientInfo>& client) {
                return client->socket_fd == client_socket;
            });
        
        if (it != clients.end()) {
            string client_name = (*it)->name;
            clients.erase(it);
            LOG_INFO(client_name + " removido do grupo", "Group-" + name);
        }
    }

    void broadcastMessage(const string& message, int exclude_socket = -1) {
        lock_guard<mutex> lock(clients_mutex);
        message_count++;
        
        for (const auto& client : clients) {
            if (client->socket_fd != exclude_socket) {
                if (send(client->socket_fd, message.c_str(), message.length(), 0) < 0) {
                    LOG_ERROR("Falha ao enviar mensagem para " + client->name, "Group-" + name);
                }
            }
        }
    }

    string getGroupInfo() const {
        lock_guard<mutex> lock(clients_mutex);
        return name + " | 👥 " + to_string(clients.size()) + " | 💬 " + to_string(message_count);
    }

    string getDetailedInfo() const {
        lock_guard<mutex> lock(clients_mutex);
        string info = "Grupo: " + name + "\n";
        info += "   ID: " + to_string(group_id) + "\n";
        info += "   Clientes conectados: " + to_string(clients.size()) + "\n";
        info += "   Total de mensagens: " + to_string(message_count) + "\n";
        
        if (!clients.empty()) {
            info += "   👥 Lista de clientes:\n";
            for (const auto& client : clients) {
                info += "      • " + client->name + " (" + client->address + ")\n";
            }
        }
        
        return info;
    }

    vector<string> getClientList() const {
        vector<string> client_list;
        lock_guard<mutex> lock(clients_mutex);
        
        for (const auto& client : clients) {
            client_list.push_back(client->name + " (" + client->address + ")");
        }
        
        return client_list;
    }
};

#endif