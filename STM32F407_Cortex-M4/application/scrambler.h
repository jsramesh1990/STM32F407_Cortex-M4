#ifndef SCRAMBLER_H
#define SCRAMBLER_H

#include <stdint.h>

#define SCRAMBLER_MAX_WORD_LENGTH   45U
#define SCRAMBLER_MAX_RESULTS       32U

typedef struct
{
    char word[SCRAMBLER_MAX_WORD_LENGTH + 1];
    uint8_t length;
} ScramblerWord_t;

/*
 * Initialize the scrambler engine.
 */
void Scrambler_Init(void);

/*
 * Solve the scrambled word.
 *
 * Returns number of matching words.
 */
uint32_t Scrambler_Solve(const char *input);

/*
 * Sort characters of a word.
 */
void Scrambler_SortWord(const char *input,
                        char *sorted_output);

/*
 * Compare two words after sorting.
 */
uint8_t Scrambler_IsAnagram(const char *word1,
                            const char *word2);

#endif
