#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

int score(string word);
int points[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
int main(void)
{
    char *word1 = get_string("pleyer 1: ");
    string word2 = get_string("pleyer 2: ");
    int p1 = score(word1);
    int p2 = score(word2);
    if (p1 > p2)
    {
        printf("Player 1 wins!\n");
    }
    else if (p2 > p1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }
}

// calculating scores
int score(string word)
{
    int score = 0;
    for (int i = 0, n = strlen(word); i < n; i++)
    {
        if (islower(word[i]) || isupper(word[i]))
        {
            score += points[toupper(word[i]) - 'A'];
        }
    }
    return score;
}
