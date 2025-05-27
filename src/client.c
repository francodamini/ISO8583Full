#include "../Include/client.h"
#include "../Include/iso8583.h"

void run_client(const char *server_ip, int port, int client_id) {
    ISO8583Message *msg = iso8583_create();

    iso8583_set_mti(msg, "0200");

    iso8583_set_field(msg, 2, "1234567890123456"); // Example card number
    iso8583_set_field(msg, 3, "000000"); // Processing code
    iso8583_set_field(msg, 4, "000000100000"); // Transaction amount
    iso8583_set_field(msg, 7, "0101010101"); // Transmission date and time
    iso8583_set_field(msg, 11, "123456"); // Systems trace audit number
    iso8583_set_field(msg, 41, "12345678"); // Terminal ID
    iso8583_set_field(msg, 42, "123456789012345"); // Merchant ID
    iso8583_set_field(msg, 49, "840"); // Currency code (USD)
    iso8583_set_field(msg, 60, "12345678901234567890"); // Additional data
    iso8583_set_field(msg, 102, "12345678901234567890"); // Cardholder data
    iso8583_set_field(msg, 103, "12345678901234567890"); // Cardholder name
    iso8583_set_field(msg, 104, "12345678901234567890"); // Cardholder address
    iso8583_set_field(msg, 105, "12345678901234567890"); // Cardholder phone number
    iso8583_set_field(msg, 106, "12345678901234567890"); // Cardholder email
    iso8583_set_field(msg, 107, "12345678901234567890"); // Cardholder ID
    iso8583_set_field(msg, 108, "12345678901234567890"); // Cardholder birth date
    iso8583_set_field(msg, 109, "12345678901234567890"); // Cardholder

    int client_sock = connect_to_server(server_ip, port);
    if (client_sock < 0) {
        perror("Failed to connect to server");
        iso8583_free(msg);
        return;
    }
    char buffer[1024];
    int len = iso8583_encode(msg, buffer, sizeof(buffer));
    if (len < 0) {
        perror("Failed to encode message");
        close_socket(client_sock);
        iso8583_free(msg);
        return;
    }
    if (send_message(client_sock, buffer, len) < 0) {
        perror("Failed to send message");
        close_socket(client_sock);
        iso8583_free(msg);
        return;
    }
    printf("Client %d sent message to server %s:%d\n", client_id, server_ip, port);
    // Wait for response
    char response[1024];
    int response_len = receive_message(client_sock, response, sizeof(response));
    if (response_len < 0) {
        perror("Failed to receive message");
    } else {
        ISO8583Message *response_msg = iso8583_decode(response, response_len);
        if (response_msg) {
            printf("Received response from server:\n");
            iso8583_print(response_msg);
            iso8583_free(response_msg);
        } else {
            printf("Failed to decode response message\n");
        }
    }
    close_socket(client_sock);
    iso8583_free(msg);
    printf("Client %d finished communication with server %s:%d\n", client_id, server_ip, port);
}   