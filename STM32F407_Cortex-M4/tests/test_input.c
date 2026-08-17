#include <stdio.h>
#include <string.h>

#include "input_manager.h"


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


static void test_valid_input(void)
{
    printf("\n--- Valid Input Tests ---\n");


    TEST_ASSERT(
        InputManager_Validate("ew") == 1
    );


    TEST_ASSERT(
        InputManager_Validate("stpo") == 1
    );


    TEST_ASSERT(
        InputManager_Validate("triangle") == 1
    );
}


static void test_invalid_input(void)
{
    printf("\n--- Invalid Input Tests ---\n");


    TEST_ASSERT(
        InputManager_Validate("") == 0
    );


    TEST_ASSERT(
        InputManager_Validate("a") == 0
    );


    TEST_ASSERT(
        InputManager_Validate("abc123") == 0
    );


    TEST_ASSERT(
        InputManager_Validate("hello!") == 0
    );
}


static void test_uppercase_input(void)
{
    printf("\n--- Uppercase Input Tests ---\n");


    TEST_ASSERT(
        InputManager_Validate("STPO") == 1
    );
}


int main(void)
{
    printf("\n");
    printf("========================================\n");
    printf(" Input Manager Unit Tests\n");
    printf(" STM32F407 Cortex-M4 Project\n");
    printf("========================================\n");


    test_valid_input();

    test_invalid_input();

    test_uppercase_input();


    printf("\n========================================\n");
    printf("Tests Passed : %d\n", tests_passed);
    printf("Tests Failed : %d\n", tests_failed);
    printf("========================================\n");


    return (tests_failed == 0) ? 0 : 1;
}
