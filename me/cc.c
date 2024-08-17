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

char* substring(const char* s, int start, int end) {
    int len = end - start + 1;
    char* substr = (char*)malloc((len + 1) * sizeof(char));
    strncpy(substr, s + start, len);
    substr[len] = '\0';
    return substr;
}

void parter(const char* s, int start, char**** result, int* returnSize, int** returnColumnSizes, char** part, int size) {
    int n = strlen(s);
    if (start == n) {
        (*returnSize)++;
        *result = (char***)realloc(*result, (*returnSize) * sizeof(char**));
        (*result)[*returnSize - 1] = (char**)malloc(size * sizeof(char*));
        (*returnColumnSizes) = (int*)realloc(*returnColumnSizes, (*returnSize) * sizeof(int));
        (*returnColumnSizes)[*returnSize - 1] = size;
        for (int i = 0; i < size; i++) {
            (*result)[*returnSize - 1][i] = strdup(part[i]);
        }
        return;
    }

    for (int end = start; end < n; end++) {
        if (check(s, start, end)) {
            part[size] = substring(s, start, end);
            parter(s, end + 1, result, returnSize, returnColumnSizes, part, size + 1);
            free(part[size]);
        }
    }
}

char*** partition(const char* s, int* returnSize, int** returnColumnSizes) {
    int n = strlen(s);
    *returnSize = 0;
    *returnColumnSizes = NULL;
    char*** result = NULL;
    char** part = (char**)malloc(n * sizeof(char*));
    parter(s, 0, &result, returnSize, returnColumnSizes, part, 0);
    free(part);
    return result;
}
