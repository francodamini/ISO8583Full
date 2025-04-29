#ifndef TRANSPORT_H
#define TRANSPORT_H

int start_server(int port);                      // Returns server socket
int accept_client(int server_sock);              // Accepts a client, returns client socket
int connect_to_server(const char *ip, int port); // Connect to remote server

int send_message(int sockfd, const char *data, int len);   // Sends length-prefixed message
int receive_message(int sockfd, char *buffer, int maxlen); // Receives message, returns length

void close_socket(int sockfd);

#endif
