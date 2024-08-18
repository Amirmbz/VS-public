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
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // initializing the number of characters our key has.
    int key_count = strlen(argv[1]);
    // see if the key has exacly 26 characters.
    if (key_count != 26)
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }
    // checking other conditions the key must have.
    for (int i = 0; i < key_count; i++)
    {
        // it should be all letters.
        if (isalpha(argv[1][i]))
        {
            // for creating an equal value for every character off our key.
            argv[1][i] = tolower(argv[1][i]);
        }
        else
        {
            printf("Key must contain 26 characters.\n");
            return 1;
        }
        // it shouldn't have any repetition.
        for (int y = 0; y < i; y++)
        {
            if (argv[1][i] == argv[1][y])
            {
                printf("Key must contain 26 characters.\n");
                return 1;
            }
        }
    }
    // storing our plaintext.
    string input = get_string("plaintext:  ");
    // replacing our plaintext with ciphertext.
    for (int i = 0, n = strlen(input); i < n; i++)
    {
        if (islower(input[i]))
        {
            input[i] = argv[1][input[i] - 'a'];
        }
        else if (isupper(input[i]))
        {
            input[i] = toupper(argv[1][input[i] - 'A']);
        }
    }
    printf("ciphertext: %s\n", input);
}
