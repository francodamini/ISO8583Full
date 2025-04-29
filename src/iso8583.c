#include "../include/iso8583.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ISO8583Message *iso8583_create(){
    ISO8583Message *msg = (ISO8583Message *)malloc(sizeof(ISO8583Message));
    if (msg == NULL) return NULL;

    memset(msg->MTI, 0, sizeof(msg->MTI));
    memset(msg->bitmap, 0, sizeof(msg->bitmap));

    for (int i = 1; i <= NUM_FIELDS; i++){
        msg->DE[i] = NULL;
    }

    return msg;
}

void iso8583_set_mti(ISO8583Message *msg, char *value) {
    if (!msg) return;
    strncpy(msg->MTI, value, sizeof(msg->MTI) - 1);
}

void iso8583_set_field(ISO8583Message *msg, int field, const char *value){
    if (field < 1 || field > NUM_FIELDS) return;
    
    if(msg->DE[field]){
        free(msg->DE[field]);
    }

    msg->DE[field] = (char *)malloc(strlen(value) + 1);
    strcpy(msg->DE[field], value);

    msg->bitmap[(field - 1)/8] |= (1 << (7 - ((field - 1) % 8)));

}

const char *iso8583_get_field(ISO8583Message *msg, int field) {
    if (field < 1 || field > NUM_FIELDS) return NULL;
    return msg->DE[field];
}

void iso8583_free(ISO8583Message *msg){
    if (msg){
        for (int i = 1; i <= NUM_FIELDS; i++){
            if (msg->DE[i]) free(msg->DE[i]);
        }
        free(msg);
    }
}

void iso8583_print(const ISO8583Message *msg) {
    printf("MTI: %s\n", msg->MTI);
    printf("Bitmap: ");
    for (int i = 0; i < 16; i++){
        for (int bit = 7; bit >= 0; bit--){
            printf("%d", (msg->bitmap[i] >> bit) & 1);
        }
    }
    printf("\n");
    for (int i = 1; i <= NUM_FIELDS; i++){
        if (msg->DE[i]) printf("Field %d: %s\n", i, msg->DE[i]);
    }
}

int main() {
    ISO8583Message *msg = iso8583_create();
    if (!msg) {
        printf("Error al crear el mensaje ISO8583.\n");
        return 1;
    }

    // Asignamos el MTI
    iso8583_set_mti(msg, "0100");
    // Establecemos algunos campos
    iso8583_set_field(msg, 2, "4000001234567899");  // PAN (Primary Account Number)
    iso8583_set_field(msg, 3, "000000");            // Processing Code
    iso8583_set_field(msg, 4, "000000010000");      // Amount, Transaction
    iso8583_set_field(msg, 7, "0429123456");        // Transmission Date & Time
    iso8583_set_field(msg, 11, "123456");           // Systems Trace Audit Number (STAN)

    // Imprimimos el mensaje
    iso8583_print(msg);

    // Liberamos la memoria
    iso8583_free(msg);

    return 0;
}

