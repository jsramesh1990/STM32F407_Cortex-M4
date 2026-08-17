#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <stdint.h>

#define INPUT_MAX_LENGTH 45U

typedef enum
{
    INPUT_INVALID = 0,
    INPUT_VALID = 1
} InputStatus_t;

void InputManager_Init(void);

InputStatus_t InputManager_Validate(const char *input);

uint8_t InputManager_GetLength(const char *input);

uint8_t InputManager_IsQuitCommand(const char *input);

#endif
