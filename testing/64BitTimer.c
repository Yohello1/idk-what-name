#include <stdio.h>
#include <stdint.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')


int main()
{
    printf("Hello world\n");

    uint8_t whack;
    uint8_t nO;
    whack = 256; //0000 0001, 1101 0001
    whack += 1;

    uint16_t idk = 65535;

    printf("%i\n", whack);
    printf("%i\n", nO);

    printf("Intentional overflow\n");

    whack = idk;
    printf("%i\n", whack);
    printf("%i\n", nO);

    printf("0: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(whack));
    printf("\n");
    printf("1: "BYTE_TO_BINARY_PATTERN, BYTE_TO_BINARY(nO));

    printf("\n");
    printf("\n");

    printf("m: "BYTE_TO_BINARY_PATTERN" "BYTE_TO_BINARY_PATTERN"\n",
           BYTE_TO_BINARY(idk>>8), BYTE_TO_BINARY(idk));

}
