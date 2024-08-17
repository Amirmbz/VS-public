#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }
    FILE *card = fopen(argv[1], "r");
    if (card == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    uint8_t buffer[512];
    int n = 0;
    char pictur_number[8] = "000.jpg";
    FILE *picture = fopen(pictur_number, "w");
    while (fread(buffer, 512, 1, card))
    {
        if (buffer[0] == 255 && buffer[1] == 216 && buffer[2] == 255 && buffer[3] > 223 &&
            buffer[3] < 240)
        {
            if (n > 0)
            {
                fclose(picture);
                for (int i = 0; i < 3; i++)
                {
                    pictur_number[2 - i] =
                        '0' +
                        (((n % (int) pow(10, i + 1)) - (n % (int) pow(10, i))) / (int) pow(10, i));
                }
                picture = fopen(pictur_number, "w");
            }
            n++;
        }
        if (n > 0)
        {
            fwrite(buffer, 512, 1, picture);
        }
    }
    fclose(card);
    fclose(picture);
}
