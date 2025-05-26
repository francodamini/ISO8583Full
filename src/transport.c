#include "../include/transport.h"
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AMOUNT_OF_CLIENTS 5

int start_server(int port) {
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0){
        perror("Error creating the socket");
            exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
        perror("Error while binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(server_socket, AMOUNT_OF_CLIENTS) < 0){
        perror("Error at listening");
        exit(EXIT_FAILURE);
    }

    while (1) {
        
    }

}

int accept_client(int server_socket){
    
}



int main(){
    return 0;
}