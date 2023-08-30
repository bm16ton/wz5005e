#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

/* Return CRC-8 of the data, using x^8 + x^2 + x
+ 1 polynomial. */
uint8_t Crc8(const void *vptr, int len)
{
    const uint8_t *data = vptr;
    uint16_t crc = 0;
    int i, j;
    for (j = len; j; j--, data++) {
        crc ^= (*data << 10);
        for(i = 10; i; i--) {
            if (crc & 0x8000)
                crc ^= (0x1070 << 3);
            crc <<= 1;
        }
    }
    return (uint8_t)(crc >> 8);
}


void convert(char *str, char *hex, int *size_hex)
{
    // convert string sequence into byte sequence
    int size_str = strlen(str);
    *size_hex = size_str/2;

    int i;
    for(i = 0; i<*size_hex; i++){
        char substr[3];
        int j = i*2;

        substr[0] = str[j];
        substr[1] = str[j+1];
        substr[2] = '\0';

        hex[i] = strtol(substr, NULL, 16);
    }
}
int F_chk_8( int bval, int cval ) {

   return ( bval + cval ) % 256;

}

void main(int argc, char* argv[])
{
    if (argc < 2)
    {
        fprintf(stderr,"USAGE: crc8 [hexa sequence]");
        exit(1);
    }
    int cval = 0;
    char* word;
    word = argv[1];
    int len;
    char outword[strlen(word)];


    convert(word, outword, &len);
    int crc = Crc8(outword, len);

    int i;
    printf("Data: ");
    for (i=0; i<len;i++)
        printf("%02X ", outword[i]);
    printf("\nCRC8: %d", crc);
    printf("\n");
}

