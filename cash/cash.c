#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int cent;
    do
    {
        cent = get_int("Change owed: ");
    }
    while (cent < 0);
    int count = cent / 25 + (cent % 25) / 10 + ((cent % 25) % 10) / 5 + ((cent % 25) % 10) % 5;
    printf(" %i \n", count);
}
