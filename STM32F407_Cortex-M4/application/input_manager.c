#include "input_manager.h"
#include <string.h>

static uint8_t is_alpha(char c)
{
    if (((c >= 'A') && (c <= 'Z')) ||
        ((c >= 'a') && (c <= 'z')))
    {
        return 1U;
    }

    return 0U;
}

void InputManager_Init(void)
{
    /* Nothing required initially */
}

uint8_t InputManager_GetLength(const char *input)
{
    if (input == NULL)
    {
        return 0U;
    }

    return (uint8_t)strlen(input);
}

InputStatus_t InputManager_Validate(const char *input)
{
    uint32_t i;
    uint32_t length;

    if (input == NULL)
    {
        return INPUT_INVALID;
    }

    length = strlen(input);

    if ((length < 2U) ||
        (length > INPUT_MAX_LENGTH))
    {
        return INPUT_INVALID;
    }

    for (i = 0U; i < length; i++)
    {
        if (!is_alpha(input[i]))
        {
            return INPUT_INVALID;
        }
    }

    return INPUT_VALID;
}

uint8_t InputManager_IsQuitCommand(const char *input)
{
    if (input == NULL)
    {
        return 0U;
    }

    if ((strcmp(input, "q") == 0) ||
        (strcmp(input, "Q") == 0))
    {
        return 1U;
    }

    return 0U;
}
