#include "dictionary.h"

static const char *dictionary[] =
{
    "stop",
    "spot",
    "post",
    "pots",
    "tops",

    "we",

    "apple",
    "orange",
    "banana",
    "triangle",

    "listen",
    "silent",
    "enlist"
};

#define DICTIONARY_COUNT \
    (sizeof(dictionary) / sizeof(dictionary[0]))

void Dictionary_Init(void)
{
    /*
     * Initial version:
     * Dictionary is already stored in
     * STM32 Flash as const data.
     */
}

uint32_t Dictionary_GetWordCount(void)
{
    return (uint32_t)DICTIONARY_COUNT;
}

const char *Dictionary_GetWord(uint32_t index)
{
    if (index >= DICTIONARY_COUNT)
    {
        return NULL;
    }

    return dictionary[index];
}
