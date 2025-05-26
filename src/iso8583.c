#include "../include/iso8583.h"
#include <stdlib.h>

//--------------------------------------------------------------------
//Constructor/Destructor

ISO8583Message *iso8583_create() {
    ISO8583Message *msg = (ISO8583Message*)malloc(sizeof(ISO8583Message));
    if (msg == NULL) return NULL;
    memset(msg->MTI, 0, sizeof(msg->MTI));
    memset(msg->bitmap, 0, sizeof(msg->bitmap));
    for(int i = 1; i <= NUM_FIELDS; i++) {
        msg->DE[i] = NULL;
    }
    return msg;
}

void iso8583_free(ISO8583Message *msg) {
    if (msg) {
        for (int i = 1; i <= NUM_FIELDS; i++) {
            if (msg->DE[i]) {
                free(msg->DE[i]);
            }
        }
        free(msg);
    }
}

//-------------------------------------------------------------
// Setters / Getters

void iso8583_set_mti(ISO8583Message *msg, const char *value) {
    if (!msg || !value) return;
    strncpy(msg->MTI, value, sizeof(msg->MTI) - 1);
    msg->MTI[sizeof(msg->MTI) - 1] = '\0'; // Ensure null termination
}

void iso8583_set_field(ISO8583Message *msg, int field, const char *value) {
    if (!msg || field < 1 || field > NUM_FIELDS || !value) return;
    if (msg->DE[field]){
        free(msg->DE[field]);
    }
    msg->DE[field] = (char*)malloc(strlen(value) + 1);
    if (msg->DE[field]) {
        strcpy(msg->DE[field], value);
    }
}

const char *iso8583_get_field(const ISO8583Message *msg, int field){
    if (!msg || field < 1 || field > NUM_FIELDS) return NULL;
    return msg->DE[field];
}

//-------------------------------------------------------------
// Encoding/Decoding

int iso8583_encode(const ISO8583Message *msg, char *buffer, int maxlen) {
    if (!msg || !buffer || maxlen <= 0) return - 1;

    int offset = 0;

    // Encode MTI
    if (offset + 5 > maxlen) return -1;
    strncpy(buffer + offset, msg->MTI, 4);

    offset += 4;

    // Encode Bitmap
    if (offset + 16 > maxlen) return -1;
    memcpy(buffer + offset, msg->bitmap, 16);
    offset += 16;

    //Encode Data Elements
    for (int i = 1; i <= NUM_FIELDS; i++) {
        if (msg->DE[i]) {
            int len = strlen(msg->DE[i]);
            if (offset + len + 2 > maxlen) return -1; // +2 for length prefix
            buffer[offset++] = (char)(len >> 8); // High byte
            buffer[offset++] = (char)(len & 0xFF); // Low byte
            memcpy(buffer + offset, msg->DE[i], len);
            offset += len;
        }
    }

    // Null-terminate the buffer
    if (offset < maxlen) {
        buffer[offset] = '\0';
    } else {
        return -1; // Buffer overflow
    }
    return offset; // Return the number of bytes written
}

ISO8583Message *iso8583_decode(const char *buffer, int lenght) {
    if (!buffer || lenght <= 0) return NULL;

    ISO8583Message *msg = iso8583_create();
    if (!msg) return NULL;

    int offset = 0;

    // Decode MTI
    if (lenght < 4) {
        iso8583_free(msg);
        return NULL;
    }
    strncpy(msg->MTI, buffer + offset, 4);
    msg->MTI[4] = '\0'; // Ensure null termination
    offset += 4;

    // Decode Bitmap
    if (lenght < offset + 16) {
        iso8583_free(msg);
        return NULL;
    }
    memcpy(msg->bitmap, buffer + offset, 16);
    offset += 16;

    // Decode Data Elements
    while (offset < lenght) {
        if (lenght - offset < 2) {
            iso8583_free(msg);
            return NULL; // Not enough data for length prefix
        }
        int len = ((unsigned char)buffer[offset] << 8) | (unsigned char)buffer[offset + 1];
        offset += 2;

        if (lenght - offset < len) {
            iso8583_free(msg);
            return NULL; // Not enough data for the field
        }

        for (int i = 1; i <= NUM_FIELDS; i++) {
            if (!msg->DE[i]) {
                msg->DE[i] = (char*)malloc(len + 1);
                if (!msg->DE[i]) {
                    iso8583_free(msg);
                    return NULL; // Memory allocation failed
                }
                strncpy(msg->DE[i], buffer + offset, len);
                msg->DE[i][len] = '\0'; // Ensure null termination
                break;
            }
        }
        offset += len;
    }

    return msg;
}

void iso8583_print(const ISO8583Message *msg) {
    if (!msg) return;

    printf("MTI: %s\n", msg->MTI);
    printf("Bitmap: ");
    for (int i = 0; i < 16; i++) {
        printf("%02X ", msg->bitmap[i]);
    }
    printf("\n");

    for (int i = 1; i <= NUM_FIELDS; i++) {
        if (msg->DE[i]) {
            printf("DE[%d]: %s\n", i, msg->DE[i]);
        }
    }
}