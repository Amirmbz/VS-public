#include <cs50.h>
#include <stdio.h>

void print_pyramid(int height);

int main(void)
{
    int height;
    do
    {
        height = get_int("height: ");
    }
    while (height < 1);
    print_pyramid(height);
}

void print_pyramid(int height) // prints a pyramid in height of a given integer "height".
{
    for (int i = 0; i < height; i++)
    {
        for (int x = 0; x < height - i - 1; x++)
        {
            printf(" ");
        }
        for (int j = 0; j < i + 1; j++)
        {
            printf("#");
        }
        printf("\n");
    }
}
