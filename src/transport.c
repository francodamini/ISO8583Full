#include "../include/transport.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#define AMOUNT_OF_CLIENTS 5

int start_server(int port) {

    int server = socket(AF_INET, SOCK_STREAM, 0);
    if (server > 0){
        struct sockaddr_in server_addr;
        server_addr.sin_family = AF_INET;
        server_addr.sin_addr.s_addr = INADDR_ANY; // Bind to all interfaces
        server_addr.sin_port = htons(port);
        if (bind(server, (struct sockaddr *)&server_addr, sizeof(server_addr)) == 0) {
            if (listen(server, AMOUNT_OF_CLIENTS) == 0) {
                return server; // Return the server socket
            }
        }
      
    }
    return -1; // Placeholder return value
}

int accept_client(int server_sock) {
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &addr_len);
    if (client_sock >= 0) {
        return client_sock; // Return the client socket
    }
    return -1; // Placeholder return value
}

int connect_to_server(const char *ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock >= 0) {
        struct sockaddr_in client_addr;
        client_addr.sin_family = AF_INET;
        client_addr.sin_port = htons(port);
        if (inet_pton(AF_INET, ip, &client_addr.sin_addr) >= 0) {
            if (connect(sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) == 0) {
                return sock; // Return the connected socket
            }
        }
    }
    return -1;
}

int send_message(int sockfd, const char *data, int len) {
    if (len <= 0) return -1;
    int total_sent = 0;
    while (total_sent < len) {
        int sent = send(sockfd, data + total_sent, len - total_sent, 0);
        if (sent < 0) {
            return -1;
        }
        total_sent += sent;
    }
}

int receive_message(int sockfd, char *buffer, int maxlen) {
    if (maxlen <= 0) return -1;
    int total_received = 0;
    while (total_received < maxlen) {
        int received = recv(sockfd, buffer + total_received, maxlen - total_received, 0);
        if (received < 0) {
            return -1;
        } else if (received == 0) {
            break;
        }
        total_received += received;
    }
}

void close_socket(int sockfd) {
    if (sockfd >= 0) {
        close(sockfd);
    }
}