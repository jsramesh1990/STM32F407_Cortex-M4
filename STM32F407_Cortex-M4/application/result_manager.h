#ifndef RESULT_MANAGER_H
#define RESULT_MANAGER_H

#include <stdint.h>

#define RESULT_MAX_COUNT 32U
#define RESULT_WORD_LENGTH 45U

void ResultManager_Init(void);

uint8_t ResultManager_AddResult(const char *word);

uint32_t ResultManager_GetCount(void);

const char *ResultManager_GetResult(uint32_t index);

void ResultManager_Clear(void);

#endif
