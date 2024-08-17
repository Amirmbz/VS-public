#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Function to check if a substring is a palindrome
bool check(const char* s, int start, int end) {
    while (start < end) {
        if (s[start] != s[end]) {
            return false;
        }
        start++;
        end--;
    }
    return true;
}

// Helper function to generate all partitions
void parted(const char* s, int start, char**** result, int* returnSize, int** returnColumnSizes, int* part, int size) {
    int n = strlen(s);
    if (start == n) {
        (*returnSize)++;
        *result = (char***)realloc(*result, (*returnSize) * sizeof(char**));
        (*result)[*returnSize - 1] = (char**)malloc(size * sizeof(char*));
        (*returnColumnSizes) = (int*)realloc(*returnColumnSizes, (*returnSize) * sizeof(int));
        (*returnColumnSizes)[*returnSize - 1] = size;
        for (int i = 0; i < size; i++) {
            int len = part[2 * i + 1] - part[2 * i] + 1;
            (*result)[*returnSize - 1][i] = (char*)malloc((len + 1) * sizeof(char));
            strncpy((*result)[*returnSize - 1][i], s + part[2 * i], len);
            (*result)[*returnSize - 1][i][len] = '\0';
        }
        return;
    }

    for (int end = start; end < n; end++) {
        if (check(s, start, end)) {
            part[2 * size] = start;
            part[2 * size + 1] = end;
            parted(s, end + 1, result, returnSize, returnColumnSizes, part, partitionSize + 1);
        }
    }
}

// Function to find all partitions
char*** partition(const char* s, int* returnSize, int** returnColumnSizes) {
    int n = strlen(s);
    char*** result = NULL;
    *returnSize = 0;
    *returnColumnSizes = NULL;
    int* part = (int*)malloc(2 * n * sizeof(int));
    parted(s, 0, &result, returnSize, returnColumnSizes, part, 0);
    free(part);
    return result;
}

// Function to free allocated memory for partitions
void freePartitions(char*** partitions, int size, int* columnSizes) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < columnSizes[i]; j++) {
            free(partitions[i][j]);
        }
        free(partitions[i]);
    }
    free(partitions);
    free(columnSizes);
}

int main() {
    const char* str = "ffffffffff";
    int returnSize;
    int* returnColumnSizes;

    char*** partitions = partition(str, &returnSize, &returnColumnSizes);

    // Print the partitions
    for (int i = 0; i < returnSize; i++) {
        for (int j = 0; j < returnColumnSizes[i]; j++) {
            printf("%s ", partitions[i][j]);
        }
        printf("\n");
    }

    // Free the allocated memory
    freePartitions(partitions, returnSize, returnColumnSizes);

    return 0;
}
