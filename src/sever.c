#include "../Include/server.h"
#include "../Include/transport.h"
#include "../Include/iso8583.h"
#include "../Include/database.h"

void run_server(int port) {
    int server = start_server(port);
    if (server < 0) {
        perror("Failed to start server");
        return;
    }
    printf("Server started on port %d\n", port);
    while (1) {
        int client = accept_client(server);
        if (client < 0) {
            perror("Failed to accept client");
            continue;
        }
        printf("Client connected\n");
        char buffer[1024];
        int total_received = 0;
        int len;
        
        while (total_received < sizeof(buffer)) {
            len = receive_message(client, buffer + total_received, sizeof(buffer) - total_received);
            if (len < 0) {
                perror("Failed to receive message");
                close_socket(client);
                break;
            }
            total_received += len;
            if (len == 0) {
                // No more data
                break;
            }
        }

        ISO8583Message *msg = iso8583_decode(buffer, len);
        if (msg) {
            printf("Received message from client:\n");

            PGconn *db_conn = db_connect("dbname=transaction_db user=postgres password=2612lfDF host=localhost");
            if (db_conn) {
                const char *pan = iso8583_get_field(msg, 2);
                const char *amount = iso8583_get_field(msg, 4);
                const char *datetime = iso8583_get_field(msg, 7);
                if (db_insert_transaction(db_conn, pan, amount, datetime)) {
                    printf("Transaction inserted successfully\n");
                } else {
                    printf("Failed to insert transaction\n");
                }
                db_disconnect(db_conn);
            } else {
                printf("Failed to connect to database\n");
            }

            iso8583_set_mti(msg, "0210"); // Set response MTI
            iso8583_set_field(msg, 39, "00"); // Set response code (00 = success)
            char response_buffer[1024];
            int response_len = iso8583_encode(msg, response_buffer, sizeof(response_buffer));
            int total_sent = 0;
            
            while (total_sent < response_len) {
                int sent = send_message(client, response_buffer + total_sent, response_len - total_sent);
                if (sent < 0) {
                    perror("Failed to send message");
                    break;
                }
                total_sent += sent;
            }

            iso8583_free(msg);
        } else {
            printf("Failed to decode message\n");
        }
    }
}