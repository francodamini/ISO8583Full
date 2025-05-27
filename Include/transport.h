#ifndef TRANSPORT_H
#define TRANSPORT_H

int start_server(int port);                      // Returns server socket
int accept_client(int server_sock);              // Accepts a client connection

int connect_to_server(const char *ip, int port); // Connect to a server

int send_message(int sockfd, const char *data, int len);   // Sends message (with length prefix)
int receive_message(int sockfd, char *buffer, int maxlen); // Receives message

void close_socket(int sockfd);

#endif // TRANSPORT_H
