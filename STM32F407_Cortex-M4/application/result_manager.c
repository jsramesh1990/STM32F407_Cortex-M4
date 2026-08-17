#include "result_manager.h"
#include <string.h>

typedef struct
{
    char word[RESULT_WORD_LENGTH + 1];
} ResultEntry_t;

static ResultEntry_t results[RESULT_MAX_COUNT];

static uint32_t result_count = 0U;

void ResultManager_Init(void)
{
    ResultManager_Clear();
}

void ResultManager_Clear(void)
{
    uint32_t i;

    result_count = 0U;

    for (i = 0U; i < RESULT_MAX_COUNT; i++)
    {
        results[i].word[0] = '\0';
    }
}

uint8_t ResultManager_AddResult(const char *word)
{
    if (word == NULL)
    {
        return 0U;
    }

    if (result_count >= RESULT_MAX_COUNT)
    {
        return 0U;
    }

    strncpy(results[result_count].word,
            word,
            RESULT_WORD_LENGTH);

    results[result_count].word[RESULT_WORD_LENGTH] = '\0';

    result_count++;

    return 1U;
}

uint32_t ResultManager_GetCount(void)
{
    return result_count;
}

const char *ResultManager_GetResult(uint32_t index)
{
    if (index >= result_count)
    {
        return NULL;
    }

    return results[index].word;
}
