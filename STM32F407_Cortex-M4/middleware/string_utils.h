#ifndef STRING_UTILS_H
#define STRING_UTILS_H

#include <stdint.h>
#include <stddef.h>


/*
 * ============================================================
 * String Utilities
 *
 * Functions required by:
 *
 *     dictionary.c
 *     scrambler.c
 *     input_manager.c
 *
 * ============================================================
 */


/*
 * Get string length.
 */
uint32_t String_Length(
    const char *string);


/*
 * Copy string.
 */
void String_Copy(
    char *destination,
    uint32_t destination_size,
    const char *source);


/*
 * Compare strings.
 */
int String_Compare(
    const char *string1,
    const char *string2);


/*
 * Compare strings ignoring case.
 */
int String_CompareIgnoreCase(
    const char *string1,
    const char *string2);


/*
 * Convert string to lowercase.
 */
void String_ToLower(
    char *string);


/*
 * Convert string to uppercase.
 */
void String_ToUpper(
    char *string);


/*
 * Check alphabetic string.
 *
 * Returns:
 *
 *     1 = valid
 *     0 = invalid
 */
uint8_t String_IsAlphabetic(
    const char *string);


/*
 * Check if character is alphabetic.
 */
uint8_t String_IsAlpha(
    char character);


/*
 * Check if character is digit.
 */
uint8_t String_IsDigit(
    char character);


/*
 * Sort characters in string.
 *
 * Example:
 *
 *     "stpo"
 *
 * becomes:
 *
 *     "opst"
 */
void String_Sort(
    char *string);


/*
 * Compare two strings after sorting.
 *
 * Used for anagram detection.
 */
uint8_t String_IsAnagram(
    const char *string1,
    const char *string2);


/*
 * Remove CR/LF.
 */
void String_RemoveNewline(
    char *string);


/*
 * Trim spaces.
 */
void String_Trim(
    char *string);

#endif /* STRING_UTILS_H */
