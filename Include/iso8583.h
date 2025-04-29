#ifndef ISO8583_H
#define ISO8583_H

#define NUM_FIELDS 128

typedef struct
{
    char MTI[5];
    unsigned char bitmap[16];
    char *DE[NUM_FIELDS + 1];
} ISO8583Message;

ISO8583Message *iso8583_create();
void iso8583_set_field(ISO8583Message *msg, int field, const char *value);
const char *iso8583_get_field(ISO8583Message *msg, int field);
void iso8583_free(ISO8583Message *msg);
void iso8583_print(const ISO8583Message *msg);

#endif