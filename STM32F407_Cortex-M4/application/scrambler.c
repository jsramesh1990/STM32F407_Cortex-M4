#include "scrambler.h"
#include "dictionary.h"
#include "result_manager.h"
#include <string.h>

static void sort_string(char *str)
{
    uint32_t i;
    uint32_t j;
    char temp;

    uint32_t length = strlen(str);

    for (i = 0U; i < length; i++)
    {
        for (j = i + 1U; j < length; j++)
        {
            if (str[i] > str[j])
            {
                temp = str[i];
                str[i] = str[j];
                str[j] = temp;
            }
        }
    }
}

void Scrambler_Init(void)
{
    Dictionary_Init();
    ResultManager_Init();
}

void Scrambler_SortWord(const char *input,
                        char *sorted_output)
{
    strcpy(sorted_output, input);

    sort_string(sorted_output);
}

uint8_t Scrambler_IsAnagram(const char *word1,
                            const char *word2)
{
    char sorted1[SCRAMBLER_MAX_WORD_LENGTH + 1];
    char sorted2[SCRAMBLER_MAX_WORD_LENGTH + 1];

    Scrambler_SortWord(word1, sorted1);
    Scrambler_SortWord(word2, sorted2);

    return (strcmp(sorted1, sorted2) == 0U);
}

uint32_t Scrambler_Solve(const char *input)
{
    uint32_t count = 0U;
    uint32_t dictionary_count;
    uint32_t i;

    char sorted_input[SCRAMBLER_MAX_WORD_LENGTH + 1];
    char sorted_word[SCRAMBLER_MAX_WORD_LENGTH + 1];

    Scrambler_SortWord(input, sorted_input);

    dictionary_count = Dictionary_GetWordCount();

    for (i = 0U; i < dictionary_count; i++)
    {
        const char *word = Dictionary_GetWord(i);

        if (word == NULL)
        {
            continue;
        }

        if (strlen(word) != strlen(input))
        {
            continue;
        }

        Scrambler_SortWord(word, sorted_word);

        if (strcmp(sorted_input, sorted_word) == 0)
        {
            ResultManager_AddResult(word);
            count++;
        }
    }

    return count;
}
