#include <stdio.h>
#include <string.h>

#include "scrambler.h"


static int tests_passed = 0;
static int tests_failed = 0;


#define TEST_ASSERT(condition)                                      \
    do                                                              \
    {                                                               \
        if (condition)                                              \
        {                                                           \
            printf("[PASS] %s\n", #condition);                     \
            tests_passed++;                                         \
        }                                                           \
        else                                                        \
        {                                                           \
            printf("[FAIL] %s\n", #condition);                    \
            tests_failed++;                                         \
        }                                                           \
    } while (0)


static void test_scrambler_initialization(void)
{
    printf("\n--- Scrambler Initialization ---\n");


    TEST_ASSERT(
        Scrambler_Init() == STATUS_OK
    );
}


static void test_basic_anagram(void)
{
    printf("\n--- Basic Anagram Tests ---\n");


    TEST_ASSERT(
        Scrambler_IsMatch("stpo", "stop") == TRUE
    );


    TEST_ASSERT(
        Scrambler_IsMatch("stpo", "pots") == TRUE
    );


    TEST_ASSERT(
        Scrambler_IsMatch("stpo", "post") == TRUE
    );


    TEST_ASSERT(
        Scrambler_IsMatch("stpo", "tops") == TRUE
    );


    TEST_ASSERT(
        Scrambler_IsMatch("stpo", "hello") == FALSE
    );
}


static void test_small_words(void)
{
    printf("\n--- Small Word Tests ---\n");


    TEST_ASSERT(
        Scrambler_IsMatch("ew", "we") == TRUE
    );


    TEST_ASSERT(
        Scrambler_IsMatch("ew", "ew") == TRUE
    );


    TEST_ASSERT(
        Scrambler_IsMatch("ab", "abc") == FALSE
    );
}


static void test_case_insensitive(void)
{
    printf("\n--- Case Insensitive Tests ---\n");


    TEST_ASSERT(
        Scrambler_IsMatch("STPO", "stop") == TRUE
    );


    TEST_ASSERT(
        Scrambler_IsMatch("stpo", "STOP") == TRUE
    );
}


int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf(" Scrambler Unit Tests\n");
    printf(" STM32F407 Cortex-M4 Project\n");
    printf("========================================\n");


    test_scrambler_initialization();

    test_basic_anagram();

    test_small_words();

    test_case_insensitive();


    printf("\n========================================\n");
    printf("Tests Passed : %d\n", tests_passed);
    printf("Tests Failed : %d\n", tests_failed);
    printf("========================================\n");


    return (tests_failed == 0) ? 0 : 1;
}
