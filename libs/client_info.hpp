#ifndef CLIENT_INFO_HPP
#define CLIENT_INFO_HPP

#include <string>
#include <netinet/in.h> 

using namespace std;

struct ClientInfo {
    int socket_fd;
    string name;
    string address;
    int current_group_id;
    
    ClientInfo(int fd, const string& client_name, const string& client_addr) 
        : socket_fd(fd), name(client_name), address(client_addr), current_group_id(-1) {}
};

#endif