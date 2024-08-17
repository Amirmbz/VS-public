#include <cs50.h>
#include <stdio.h>

void print_pyramid(int height, int spaces);

int main(void)
{
    int height;
    do
    {
        height = get_int("height: ");
    }
    while (height < 1 || height > 8);

    for (int i = 0; i < height; i++)
    {
        print_pyramid(i + 1, height - i - 1);
        printf("\n");
    }
}

void print_pyramid(int height,
                   int spaces) // prints a pyramid in the height of a given integer "height".
{

    for (int i = 0; i < spaces; i++)
    {
        printf(" ");
    }
    for (int j = 0; j < 2; j++) // in each row we need two sets of identical "#", so instead of
                                // repeating it, i've put it in a loop.
    {
        for (int z = 0; z < height; z++)
        {
            printf("#");
        }
        if (j == 0) // the check50 was not happy about having "  " AFTER each row, so based on
                    // my current knowledge, i've put a condition that "  " only applies in the
                    // first duration of the loop.
        {
            printf("  ");
        }
    }
}
