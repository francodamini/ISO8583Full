#ifndef ISO8583_H
#define ISO8583_H

#define NUM_FIELDS 128

typedef struct
{
    char MTI[5];              // Message Type Identifier
    unsigned char bitmap[16]; // Primary + Secondary Bitmap (128 bits)
    char *DE[NUM_FIELDS + 1]; // Data Elements 1–128 (index 1–128)
} ISO8583Message;

// Constructor/Destructor
ISO8583Message *iso8583_create();
void iso8583_free(ISO8583Message *msg);

// Setters / Getters
void iso8583_set_mti(ISO8583Message *msg, const char *value);
void iso8583_set_field(ISO8583Message *msg, int field, const char *value);
const char *iso8583_get_field(const ISO8583Message *msg, int field);

// Encoding/Decoding
int iso8583_encode(const ISO8583Message *msg, char *buffer, int maxlen);
ISO8583Message *iso8583_decode(const char *buffer, int length);

// Debug
void iso8583_print(const ISO8583Message *msg);

#endif