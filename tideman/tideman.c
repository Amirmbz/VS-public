#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
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
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            for (int j = 0; j < rank; j++)
            {
                if (ranks[j] == ranks[rank])
                {
                    return false;
                }
            }
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < i; j++)
        {
            preferences[ranks[j]][ranks[i]]++;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < i; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
            }
            else
            {
                break;
            }
            pair_count++;
        }
    }
}

// Merge sort
void sort(int n, pair final[])
{
    if (n / 2 == 0)
    {
        return;
    }
    pair left[(n + 1) / 2];
    pair right[n / 2];
    for (int i = 0; (n + 1) / 2 > i; i++)
    {
        left[i] = final[i];
    }
    for (int i = 0; n / 2 > i; i++)
    {
        right[i] = final[(n + 1) / 2 + i];
    }
    sort((n + 1) / 2, left);
    sort(n / 2, right);
    for (int i = 0, r = 0, l = 0; n > i; i++)
    {
        if (!(l < (n + 1) / 2))
        {
            for (; n > i; i++)
            {
                final[i] = right[r];
                r++;
            }
        }
        else if (!(r < n / 2))
        {
            for (; n > i; i++)
            {
                final[i] = left[l];
                l++;
            }
        }
        else if (preferences[right[r].winner][right[r].loser] <
                 preferences[left[l].winner][left[l].loser])
        {
            final[i] = left[l];
            l++;
        }
        else
        {
            final[i] = right[r];
            r++;
        }
    }
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    sort(pair_count, pairs);
}

// Check if there is a loop
bool loop(int initial, int next)
{
    for (int i = 0; i < initial; i++)
    {
        if (pairs[next].loser == pairs[i].winner)
        {
            if (pairs[i].loser == pairs[initial].winner)
            {
                return true;
            }
            if (loop(initial, i))
            {
                return true;
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        locked[pairs[i].winner][pairs[i].loser] = true;
        for (int j = 0; j < i; j++)
        {
            if (locked[pairs[i].loser][pairs[j].loser])
            {
                if (loop(i, j))
                {
                    locked[pairs[i].winner][pairs[i].loser] = false;
                    break;
                }
            }
        }
    }
}

// Print the winner of the election
void print_winner(void)
{
    bool winner[candidate_count];
    for (int i = 0; i < candidate_count; i++)
    {
        winner[i] = true;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == true)
            {
                winner[i] = false;
                break;
            }
        }
    }
    for (int i = 0; i < candidate_count; i++)
    {
        if (winner[i] == true)
        {
            for (int j = 0; j < candidate_count; j++)
            {
                if (locked[i][j] == true)
                {
                    printf("%s\n", candidates[i]);
                    return;
                }
            }
        }
    }
}
