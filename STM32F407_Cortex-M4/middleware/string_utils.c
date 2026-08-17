#include "string_utils.h"


/*
 * ------------------------------------------------------------
 * Internal helper
 * ------------------------------------------------------------
 */

static char String_ToLowerChar(
    char character)
{
    if ((character >= 'A') &&
        (character <= 'Z'))
    {
        return
            (char)(character + ('a' - 'A'));
    }

    return character;
}


/*
 * Get string length.
 */

uint32_t String_Length(
    const char *string)
{
    uint32_t length = 0U;

    if (string == NULL)
    {
        return 0U;
    }

    while (string[length] != '\0')
    {
        length++;
    }

    return length;
}


/*
 * Copy string.
 */

void String_Copy(
    char *destination,
    uint32_t destination_size,
    const char *source)
{
    uint32_t index;

    if ((destination == NULL) ||
        (source == NULL) ||
        (destination_size == 0U))
    {
        return;
    }

    for (index = 0U;
         index < destination_size - 1U;
         index++)
    {
        destination[index] =
            source[index];

        if (source[index] == '\0')
        {
            return;
        }
    }

    destination[
        destination_size - 1U
    ] = '\0';
}


/*
 * Compare strings.
 */

int String_Compare(
    const char *string1,
    const char *string2)
{
    if ((string1 == NULL) ||
        (string2 == NULL))
    {
        return -1;
    }

    while ((*string1 != '\0') &&
           (*string1 == *string2))
    {
        string1++;
        string2++;
    }

    return
        (unsigned char)*string1 -
        (unsigned char)*string2;
}


/*
 * Compare ignoring case.
 */

int String_CompareIgnoreCase(
    const char *string1,
    const char *string2)
{
    char c1;
    char c2;

    if ((string1 == NULL) ||
        (string2 == NULL))
    {
        return -1;
    }

    while (*string1 != '\0')
    {
        c1 =
            String_ToLowerChar(*string1);

        c2 =
            String_ToLowerChar(*string2);

        if (c1 != c2)
        {
            return
                (unsigned char)c1 -
                (unsigned char)c2;
        }

        string1++;
        string2++;
    }

    return
        (unsigned char)String_ToLowerChar(
            *string1) -
        (unsigned char)String_ToLowerChar(
            *string2);
}


/*
 * Convert to lowercase.
 */

void String_ToLower(
    char *string)
{
    if (string == NULL)
    {
        return;
    }

    while (*string != '\0')
    {
        if ((*string >= 'A') &&
            (*string <= 'Z'))
        {
            *string =
                (char)(*string +
                ('a' - 'A'));
        }

        string++;
    }
}


/*
 * Convert to uppercase.
 */

void String_ToUpper(
    char *string)
{
    if (string == NULL)
    {
        return;
    }

    while (*string != '\0')
    {
        if ((*string >= 'a') &&
            (*string <= 'z'))
        {
            *string =
                (char)(*string -
                ('a' - 'A'));
        }

        string++;
    }
}


/*
 * Check alphabetic character.
 */

uint8_t String_IsAlpha(
    char character)
{
    if (((character >= 'A') &&
         (character <= 'Z')) ||
        ((character >= 'a') &&
         (character <= 'z')))
    {
        return 1U;
    }

    return 0U;
}


/*
 * Check digit.
 */

uint8_t String_IsDigit(
    char character)
{
    if ((character >= '0') &&
        (character <= '9'))
    {
        return 1U;
    }

    return 0U;
}


/*
 * Check complete string for alphabetic characters.
 */

uint8_t String_IsAlphabetic(
    const char *string)
{
    if ((string == NULL) ||
        (*string == '\0'))
    {
        return 0U;
    }

    while (*string != '\0')
    {
        if (!String_IsAlpha(*string))
        {
            return 0U;
        }

        string++;
    }

    return 1U;
}


/*
 * Sort characters using bubble sort.
 *
 * For this project word length is only
 * 2-45 characters, so this is sufficient.
 */

void String_Sort(
    char *string)
{
    uint32_t length;
    uint32_t i;
    uint32_t j;

    char temporary;

    if (string == NULL)
    {
        return;
    }

    length =
        String_Length(string);

    for (i = 0U;
         i < length;
         i++)
    {
        for (j = 0U;
             j < (length - i - 1U);
             j++)
        {
            if (string[j] >
                string[j + 1U])
            {
                temporary =
                    string[j];

                string[j] =
                    string[j + 1U];

                string[j + 1U] =
                    temporary;
            }
        }
    }
}


/*
 * Check whether two strings are anagrams.
 */

uint8_t String_IsAnagram(
    const char *string1,
    const char *string2)
{
    char buffer1[64];
    char buffer2[64];

    uint32_t length1;
    uint32_t length2;

    if ((string1 == NULL) ||
        (string2 == NULL))
    {
        return 0U;
    }

    length1 =
        String_Length(string1);

    length2 =
        String_Length(string2);

    /*
     * Different lengths cannot be anagrams.
     */

    if (length1 != length2)
    {
        return 0U;
    }

    /*
     * Maximum supported word length
     * is 45 characters.
     */

    if ((length1 >= sizeof(buffer1)) ||
        (length2 >= sizeof(buffer2)))
    {
        return 0U;
    }

    String_Copy(
        buffer1,
        sizeof(buffer1),
        string1
    );

    String_Copy(
        buffer2,
        sizeof(buffer2),
        string2
    );

    String_ToLower(buffer1);

    String_ToLower(buffer2);

    String_Sort(buffer1);

    String_Sort(buffer2);

    if (String_Compare(
            buffer1,
            buffer2) == 0)
    {
        return 1U;
    }

    return 0U;
}


/*
 * Remove newline characters.
 */

void String_RemoveNewline(
    char *string)
{
    if (string == NULL)
    {
        return;
    }

    while (*string != '\0')
    {
        if ((*string == '\r') ||
            (*string == '\n'))
        {
            *string = '\0';

            return;
        }

        string++;
    }
}


/*
 * Trim leading and trailing spaces.
 */

void String_Trim(
    char *string)
{
    char *start;
    char *end;

    uint32_t length;

    if (string == NULL)
    {
        return;
    }

    /*
     * Remove leading spaces.
     */

    start = string;

    while ((*start == ' ') ||
           (*start == '\t'))
    {
        start++;
    }

    if (start != string)
    {
        char *source = start;
        char *destination = string;

        while (*source != '\0')
        {
            *destination++ =
                *source++;
        }

        *destination = '\0';
    }

    /*
     * Remove trailing spaces.
     */

    length =
        String_Length(string);

    if (length == 0U)
    {
        return;
    }

    end =
        &string[length - 1U];

    while ((end >= string) &&
           ((*end == ' ') ||
            (*end == '\t')))
    {
        *end = '\0';

        if (end == string)
        {
            break;
        }

        end--;
    }
}
