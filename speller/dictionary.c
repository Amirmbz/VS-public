// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <strings.h>
#include <math.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Indicate an intiger for the number of words we counting
int words;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26 * 26 * 26;
// Hash table
node *table[N];

void table_NULL(void)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    node *ptr = table[hash(word)];
    while (!(ptr == NULL))
    {
        if (strcasecmp(ptr->word,word) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int a = 0;
    for (int i = 0; i < 3; i++)
    {
        if (!(isalpha(word[i])))
        {
            break;
        }
        a += (toupper(word[i]) - 'A') * pow(26, 2 - i);
    }
    return a;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    words = 0;
    FILE *source = fopen(dictionary, "r");
    if (source == NULL)
    {
        fclose(source);
        return false;
    }
    char c;
    char word[LENGTH + 1];
    int index = 0;
    while (fread(&c, sizeof(char), 1, source))
    {
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            word[index] = c;
            index++;
            if (index > LENGTH)
            {
                while (fread(&c, sizeof(char), 1, source) && isalpha(c));
                index = 0;
            }
        }
        else if (isdigit(c))
        {
            while (fread(&c, sizeof(char), 1, source) && isalnum(c));
            index = 0;
        }
        else if (index > 0)
        {
            words++;
            word[index] = '\0';
            node *n = malloc(sizeof(node));
            for (int i = 0; i < index + 1; i++)
            {
                n->word[i] = word[i];
            }
            n->next = table[hash(word)];
            table[hash(word)] = n;
            index = 0;
        }
    }
    fclose(source);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *ptr;
    node *next;
    for (int i = 0; i < N; i++)
    {
        ptr = table[i];
        next = NULL;
        while (!(ptr == NULL))
        {
            next = ptr->next;
            free(ptr);
            ptr = next;
        }
    }
    return true;
}
