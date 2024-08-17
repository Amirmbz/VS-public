#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int main(void)
{
    string text = get_string("Text: ");
    int words = 0;
    int sentences = 0;
    int letters = 0;
    // counting letters, sentences, and words.
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        if (text[i] == ' ')
        {
            words++;
        }
        if ((text[i] == '.') || (text[i] == '!') || (text[i] == '?'))
        {
            sentences++;
        }
        if (isupper(text[i]) || islower(text[i]))
        {
            letters++;
        }
    }
    words++;
    float l = ((float) letters / words) * 100;
    float s = ((float) sentences / words) * 100;
    float g = 0.0588 * l - 0.296 * s - 15.8;
    int grade = round(g);
    if (grade > 0 && grade < 16)
    {
        printf("Grade %i\n", grade);
    }
    else if (grade > 15)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Before Grade 1\n");
    }
}
