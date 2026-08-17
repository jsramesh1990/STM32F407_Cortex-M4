#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdint.h>

#define DICTIONARY_MAX_WORDS 12000U
#define DICTIONARY_MAX_LENGTH 45U

void Dictionary_Init(void);

uint32_t Dictionary_GetWordCount(void);

const char *Dictionary_GetWord(uint32_t index);

#endif
