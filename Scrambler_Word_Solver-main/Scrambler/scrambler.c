#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_LEN 100

char **words = NULL;
int count = 0;
int capacity = 1000;

void sortString(char *str) {
    int n = strlen(str);

    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (str[i] > str[j]) {
                char temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

void trimNewline(char *str) {
    str[strcspn(str, "\n")] = '\0';
    str[strcspn(str, "\r")] = '\0';
}

int isValidWord(char *str) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (!isalpha((unsigned char)str[i])) {
            return 0;
        }
        str[i] = tolower((unsigned char)str[i]);
    }
    return 1;
}

void addWord(char *word) {
    if (count >= capacity) {
        capacity *= 2;
        words = realloc(words, capacity * sizeof(char *));
        if (!words) {
            printf("Memory allocation failed\n");
            exit(1);
        }
    }

    words[count] = malloc(strlen(word) + 1);
    strcpy(words[count], word);
    count++;
}

int main() {
    FILE *file;
    char line[MAX_LEN];

    words = malloc(capacity * sizeof(char *));
    if (!words) {
        printf("Memory allocation failed\n");
        return 1;
    }

    file = fopen("words.txt", "r");
    if (!file) {
        printf("Could not open words.txt\n");
        return 1;
    }

    // Load dictionary
    while (fgets(line, sizeof(line), file)) {
        trimNewline(line);

        int len = strlen(line);

        if (len < 2 || len > 45) continue;
        if (!isValidWord(line)) continue;

        addWord(line);
    }

    fclose(file);

    printf("Loaded words: %d\n", count);

    char input[MAX_LEN];

    while (1) {
        printf("\nEnter scrambled word (2-45 letters, q to quit): ");
        scanf("%99s", input);

        if (strcmp(input, "q") == 0) {
            printf("Exiting...\n");
            break;
        }

        if (!isValidWord(input)) {
            printf("Only alphabet letters allowed\n");
            continue;
        }

        int inputLen = strlen(input);
        if (inputLen < 2 || inputLen > 45) {
            printf("Word length must be between 2 and 45\n");
            continue;
        }

        char sortedInput[MAX_LEN];
        strcpy(sortedInput, input);
        sortString(sortedInput);

        int found = 0;

        printf("\nInput: %s\n", input);
        printf("Matches:\n");

        for (int i = 0; i < count; i++) {

            // length must match
            if (strlen(words[i]) != inputLen) continue;

            char temp[MAX_LEN];
            strcpy(temp, words[i]);
            sortString(temp);

            if (strcmp(temp, sortedInput) == 0) {
                printf("- %s\n", words[i]);
                found = 1;
            }
        }

        if (!found) {
            printf("No match found\n");
        }
    }

    // free memory
    for (int i = 0; i < count; i++) {
        free(words[i]);
    }
    free(words);

    return 0;
}
