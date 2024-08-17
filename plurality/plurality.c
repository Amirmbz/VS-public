#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// Candidates have name and vote count
typedef struct
{
    string name;
    int votes;
} candidate;

// Array of candidates
candidate candidates[MAX];

// Number of candidates
int candidate_count;

// Function prototypes
bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    // Loop over all voters
    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        // Check for invalid vote
        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    // Display winner of election
    print_winner();
}

// Update vote totals given a new vote
bool vote(string name)
{
    for (int i = 0; candidate_count > i; i++)
    {
        if (strcmp(candidates[i].name, name) == 0)
        {
            candidates[i].votes++;
            return true;
        }
    }
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    void sort(int n, candidate final[]);
    sort(candidate_count, candidates);
    for (int i = 0; candidate_count > i; i++)
    {
        printf("%s\n", candidates[i].name);
        if (candidates[i].votes > candidates[i + 1].votes)
        {
            return;
        }
    }
    return;
}

void sort(int n, candidate final[])
{
    if (n / 2 == 0)
    {
        return;
    }
    candidate left[(n + 1) / 2];
    candidate right[n - (n + 1) / 2];
    for (int i = 0; (n + 1) / 2 > i; i++)
    {
        left[i] = final[i];
    }
    for (int i = 0; n - (n + 1) / 2 > i; i++)
    {
        right[i] = final[(n + 1) / 2 + i];
    }
    sort((n + 1) / 2, left);
    sort(n - (n + 1) / 2, right);
    for (int i = 0, r = 0, l = 0; n > i; i++)
    {
        if (right[r].votes < left[l].votes)
        {
            final[i] = left[l];
            l++;
        }
        else if (right[r].votes > left[l].votes)
        {
            final[i] = right[r];
            r++;
        }
        else
        {
            final[i] = left[l];
            final[i + 1] = right[r];
            l++;
            r++;
            i++;
        }
    }
}
