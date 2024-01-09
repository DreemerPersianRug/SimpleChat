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
    int server;

    struct sockaddr_in server_addres;
    client = socket(AF_INET, SOCK_STREAM, 0);
    if(client < 0) {
        std::cout 
            << RED << SERVER << DEFAULT_COLOR 
            << "Establishing socket error." 
            << std::endl;
        exit(0);
    }

    server_addres.sin_port = htons(DEFAULT_PORT);
    server_addres.sin_family = AF_INET;
    server_addres.sin_addr.s_addr = htons(INADDR_ANY);

    std::cout 
        << GREEN << SERVER << DEFAULT_COLOR 
        << "Socket for server was successfully created." 
        << std::endl;
    
    socklen_t size = sizeof(server_addres);
    int ret = bind(client, reinterpret_cast<struct sockaddr*>(&server_addres), size);

    if(ret < 0) {
        std::cout
            << RED << SERVER << DEFAULT_COLOR
            << "Error binding connection. Socket has already been establishing!"
            << std::endl;
        return -1;
    }

    std::cout 
        << GREEN << SERVER << DEFAULT_COLOR
        << "Listening clients..."
        << DEFAULT_COLOR
        << std::endl;
    listen(client, 1);

    server = accept(client, reinterpret_cast<struct sockaddr*>(&server_addres), &size);
    if(server < 0) {
        std::cout 
            << RED << SERVER << DEFAULT_COLOR
            << "Can't accepting client."
            << std::endl;
    }

    char buffer[BUFFER_SIZE];
    if (server > 0) {
        strcpy(buffer, "Server connected!");
        send(server, buffer, BUFFER_SIZE, 0);
        std::cout 
            << GREEN << SERVER << DEFAULT_COLOR
            << "Client connected."
            << std::endl
            << RED
            << "Enter `" << STOP_SYMB << "` to end connection."
            << DEFAULT_COLOR
            << std::endl;
        std::cout << CYAN << CLIENT << DEFAULT_COLOR;
        recv(server, buffer, BUFFER_SIZE, 0);
        std::cout << buffer << std::endl;

        if (is_client_connection_close(buffer)) {
            exit(0);
        }

        while(true) {
            //PASS
            std::cout << BLUE << SERVER << DEFAULT_COLOR;
            std::cin.getline(buffer, BUFFER_SIZE);
            send(server, buffer, BUFFER_SIZE, 0);
            if(is_client_connection_close(buffer)) {
                break;
            }

            std::cout << CYAN << CLIENT << DEFAULT_COLOR;
            recv(server, buffer, BUFFER_SIZE, 0);
            std::cout << buffer << std::endl;
            if(is_client_connection_close(buffer)) {
                break;
            }
        }
        std::cout << MAGENTA << std::endl
        << "The connected has been stopped!"
        << DEFAULT_COLOR << std::endl;
        close(client);
        exit(0);
    }
    return 0;    
} 