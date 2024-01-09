#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN "\033[36m"
#define DEFAULT_COLOR "\033[00m"

#define SERVER "[Server]: "
#define CLIENT "[Client]: "
#define STOP_SYMB '~'

#define BUFFER_SIZE 2048

#define DEFAULT_PORT 1602
#define SERVER_IP "127.0.0.1"

bool is_client_connection_close(const char* msg) {
    for (size_t i = 0; i < strlen(msg); i++) {
        if(msg[i] == STOP_SYMB) {
            return true;
        }
    }
    return false;
}

int main(int argc, char const* argv[]) {
    int client;

    struct sockaddr_in server_addres;
    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0) {
        std::cout 
            << RED << CLIENT << DEFAULT_COLOR 
            << "Establishing socket error." 
            << std::endl;
        exit(0);
    }
    server_addres.sin_port = htons(DEFAULT_PORT);
    server_addres.sin_family = AF_INET;
    inet_pton(AF_INET, SERVER_IP, &server_addres.sin_addr);

    std::cout 
        << GREEN << CLIENT << DEFAULT_COLOR 
        << "Socket for client was successfully created." 
        << std::endl;

    socklen_t size = sizeof(server_addres);
    int ret = connect(client, reinterpret_cast<struct sockaddr*>(&server_addres), size);
    if(ret == 0) {
        std::cout 
            << GREEN << CLIENT << DEFAULT_COLOR 
            << "Connection to server "
            << BLUE << SERVER_IP  << DEFAULT_COLOR 
            << " with port number " 
            << BLUE << DEFAULT_PORT << DEFAULT_COLOR
            << std::endl;
    }

    std::cout 
        << GREEN << CLIENT << DEFAULT_COLOR 
        << "Waiting to server confirmation..."
        << DEFAULT_COLOR
        << std::endl;

    char buffer[BUFFER_SIZE];
    recv(client, buffer, BUFFER_SIZE, 0);
    std::cout 
            << GREEN << SERVER << DEFAULT_COLOR 
            << buffer
            << std::endl
            << RED
            << "Enter `" << STOP_SYMB << "` to end connection."
            << DEFAULT_COLOR
            << std::endl;
    while(true) {
        std::cout << BLUE << CLIENT << DEFAULT_COLOR;
        std::cin.getline(buffer, BUFFER_SIZE);
        send(client, buffer, BUFFER_SIZE, 0);
        if(is_client_connection_close(buffer)) {
            break;
        }

        std::cout << CYAN << SERVER << DEFAULT_COLOR;
        recv(client, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;
        if(is_client_connection_close(buffer)) {
            break;
        }
    }
    std::cout << MAGENTA << std::endl
        << "The connected has been stopped!"
        << DEFAULT_COLOR << std::endl;
    close(client);
    //is_exit_flag = false;
    exit(0);
    return 0;
}