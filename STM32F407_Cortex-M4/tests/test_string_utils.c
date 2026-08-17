#include <stdio.h>
#include <string.h>

#include "string_utils.h"


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
            printf("[FAIL] %s\n", #condition);                     \
            tests_failed++;                                         \
        }                                                           \
    } while (0)


static void test_string_length(void)
{
    printf("\n--- String Length Tests ---\n");

    TEST_ASSERT(
        String_Length("hello") == 5U
    );

    TEST_ASSERT(
        String_Length("") == 0U
    );

    TEST_ASSERT(
        String_Length("STM32F407") == 9U
    );
}


static void test_string_alpha(void)
{
    printf("\n--- Alphabetic Tests ---\n");

    TEST_ASSERT(
        String_IsAlpha('A') == 1U
    );

    TEST_ASSERT(
        String_IsAlpha('z') == 1U
    );

    TEST_ASSERT(
        String_IsAlpha('5') == 0U
    );

    TEST_ASSERT(
        String_IsAlpha('@') == 0U
    );

    TEST_ASSERT(
        String_IsAlphabetic("hello") == 1U
    );

    TEST_ASSERT(
        String_IsAlphabetic("abcXYZ") == 1U
    );

    TEST_ASSERT(
        String_IsAlphabetic("abc123") == 0U
    );

    TEST_ASSERT(
        String_IsAlphabetic("hello!") == 0U
    );
}


static void test_lower_upper(void)
{
    char buffer1[] = "HELLO";

    char buffer2[] = "hello";


    printf("\n--- Case Conversion Tests ---\n");


    String_ToLower(buffer1);

    TEST_ASSERT(
        strcmp(buffer1, "hello") == 0
    );


    String_ToUpper(buffer2);

    TEST_ASSERT(
        strcmp(buffer2, "HELLO") == 0
    );
}


static void test_sort(void)
{
    char buffer[] = "stpo";


    printf("\n--- String Sort Tests ---\n");


    String_Sort(buffer);

    TEST_ASSERT(
        strcmp(buffer, "opst") == 0
    );
}


static void test_anagram(void)
{
    printf("\n--- Anagram Tests ---\n");

    TEST_ASSERT(
        String_IsAnagram("stpo", "stop") == 1U
    );

    TEST_ASSERT(
        String_IsAnagram("stpo", "pots") == 1U
    );

    TEST_ASSERT(
        String_IsAnagram("stpo", "post") == 1U
    );

    TEST_ASSERT(
        String_IsAnagram("stpo", "tops") == 1U
    );

    TEST_ASSERT(
        String_IsAnagram("hello", "world") == 0U
    );

    TEST_ASSERT(
        String_IsAnagram("abc", "abcd") == 0U
    );
}


int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf(" String Utilities Unit Tests\n");
    printf(" STM32F407 Cortex-M4 Project\n");
    printf("========================================\n");


    test_string_length();

    test_string_alpha();

    test_lower_upper();

    test_sort();

    test_anagram();


    printf("\n========================================\n");
    printf("Tests Passed : %d\n", tests_passed);
    printf("Tests Failed : %d\n", tests_failed);
    printf("========================================\n");


    return (tests_failed == 0) ? 0 : 1;
}
