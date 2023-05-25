#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORD_LENGTH 100
#define MAX_LINE_LENGTH 1000

struct WordFrequency {
    char word[MAX_WORD_LENGTH];
    int frequency;
};

char** find_frequent_words(const char* path, int32_t n) {
    FILE* file = fopen(path, "r");
    if (file == NULL) {
    	fprintf(stderr, "Failed to open file: %s\n", path);
    	return NULL;
     }

    struct WordFrequency* wordFrequencies;
    int uniqueWordCount = 0;
    int maxWordCount = 10000;

    wordFrequencies = malloc(maxWordCount * sizeof(struct WordFrequency));
    if (wordFrequencies == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
    	fclose(file);
    	return NULL;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        char* word = strtok(line, " \t\n");
        while (word != NULL) {
            int existingIndex = -1;
            for (int i = 0; i < uniqueWordCount; i++) {
                if (strcmp(wordFrequencies[i].word, word) == 0) {
                    existingIndex = i;
                    break;
                }
            }

            if (existingIndex >= 0) {
                wordFrequencies[existingIndex].frequency++;
            } else {
                if (uniqueWordCount == maxWordCount) {
                    maxWordCount *= 2;
                    struct WordFrequency* newWordFrequencies = realloc(wordFrequencies, maxWordCount * sizeof(struct WordFrequency));
                    if (newWordFrequencies == NULL) {
                        fprintf(stderr, "Memory allocation failed.\n");
                        fclose(file);
                        free(wordFrequencies);
                        return NULL;
                    }
                    wordFrequencies = newWordFrequencies;
                }
                strncpy(wordFrequencies[uniqueWordCount].word, word, sizeof(wordFrequencies[uniqueWordCount].word) - 1);
                wordFrequencies[uniqueWordCount].word[sizeof(wordFrequencies[uniqueWordCount].word) - 1] = '\0';
                wordFrequencies[uniqueWordCount].frequency = 1;
                uniqueWordCount++;
            }

            word = strtok(NULL, " \t\n");
        }
    }
    fclose(file);

    for (int i = 0; i < uniqueWordCount - 1; i++) {
        for (int j = 0; j < uniqueWordCount - i - 1; j++) {
            if (wordFrequencies[j].frequency < wordFrequencies[j + 1].frequency) {
                struct WordFrequency temp = wordFrequencies[j];
                wordFrequencies[j] = wordFrequencies[j + 1];
                wordFrequencies[j + 1] = temp;
            }
        }
    }

    int resultCount = (n < uniqueWordCount) ? n : uniqueWordCount;
    char** frequentWords = malloc((resultCount + 1) * sizeof(char*));
    if (frequentWords == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        free(wordFrequencies);
        return NULL;
    }

    for (int i = 0; i < resultCount; i++) {
        frequentWords[i] = strdup(wordFrequencies[i].word);
        if (frequentWords[i] == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            for (int j = 0; j < i; j++) {free(frequentWords[j]);}
            free(frequentWords);
            free(wordFrequencies);
            return NULL;
        }
    }
    frequentWords[resultCount] = NULL;

    free(wordFrequencies);

    return frequentWords;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <file_path> <n>\n", argv[0]);
        return 1;
    }

    const char* path = argv[1];
    int32_t n = atoi(argv[2]);
    if (n <= 0) {
        fprintf(stderr, "Invalid value for n: %s\n", argv[2]);
        return 1;
    }

    char** frequentWords = find_frequent_words(path, n);
    if (frequentWords == NULL) {
        return 1;
    }

    printf("The %d most frequent words:\n", n);
    for (int i = 0; frequentWords[i] != NULL; i++) {printf("%s\n", frequentWords[i]);}

    for (int i = 0; frequentWords[i] != NULL; i++) {free(frequentWords[i]);}
    free(frequentWords);

    return 0;
}

