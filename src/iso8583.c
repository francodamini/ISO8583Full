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
    printf("Bitmap: %s\n", msg->bitmap);
    for (int i = 1; i <= NUM_FIELDS; i++){
        if (msg->DE[i]) printf("Field %d: %s\n", i, msg->DE[i]);
    }
}
