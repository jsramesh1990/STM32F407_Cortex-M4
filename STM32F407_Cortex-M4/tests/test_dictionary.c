#include <stdio.h>
#include <string.h>

#include "dictionary.h"


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


static void test_dictionary_init(void)
{
    printf("\n--- Dictionary Initialization ---\n");


    TEST_ASSERT(
        Dictionary_Init() == STATUS_OK
    );
}


static void test_dictionary_add(void)
{
    printf("\n--- Dictionary Add Tests ---\n");


    TEST_ASSERT(
        Dictionary_Add("stop") == STATUS_OK
    );


    TEST_ASSERT(
        Dictionary_Add("pots") == STATUS_OK
    );


    TEST_ASSERT(
        Dictionary_Add("post") == STATUS_OK
    );


    TEST_ASSERT(
        Dictionary_Add("tops") == STATUS_OK
    );
}


static void test_dictionary_find(void)
{
    printf("\n--- Dictionary Search Tests ---\n");


    TEST_ASSERT(
        Dictionary_Find("stop") == TRUE
    );


    TEST_ASSERT(
        Dictionary_Find("pots") == TRUE
    );


    TEST_ASSERT(
        Dictionary_Find("post") == TRUE
    );


    TEST_ASSERT(
        Dictionary_Find("hello") == FALSE
    );
}


static void test_dictionary_count(void)
{
    uint32_t count;


    printf("\n--- Dictionary Count Test ---\n");


    count =
        Dictionary_GetCount();


    TEST_ASSERT(
        count >= 4U
    );
}


int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf(" Dictionary Unit Tests\n");
    printf(" STM32F407 Cortex-M4 Project\n");
    printf("========================================\n");


    test_dictionary_init();

    test_dictionary_add();

    test_dictionary_find();

    test_dictionary_count();


    printf("\n========================================\n");
    printf("Tests Passed : %d\n", tests_passed);
    printf("Tests Failed : %d\n", tests_failed);
    printf("========================================\n");


    return (tests_failed == 0) ? 0 : 1;
}
