#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, string argv[])
{
    // check if there is only one argument.
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
    // check if the argument is all intigers(digits).
    for (int i = 0, n = strlen(argv[1]); i < n; i++)
    {
        if (isdigit(argv[1][i]))
            ;
        else
        {
            printf("Usage: ./caesar key\n");
            return 1;
        }
    }
    // converting the argument to an intiger.
    int key = atoi(argv[1]);
    string input = get_string("plaintext:  ");
    printf("ciphertext: ");
    // converting plaintext to ciphertext.
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (isupper(input[i]))
        {
            printf("%c", ((input[i] - 'A' + key) % 26) + 'A');
        }
        else if (islower(input[i]))
        {
            printf("%c", ((input[i] - 'a' + key) % 26) + 'a');
        }
        else
        {
            printf("%c", input[i]);
        }
    }
    printf("\n");
}
