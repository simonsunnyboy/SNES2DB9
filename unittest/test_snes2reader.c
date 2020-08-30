/**
 * @file   example_unittest.c
 *
 * ASM Software Unittest Framework
 * (c) 2016 by Matthias Arndt <marndt@asmsoftware.de>
 *
 * The MIT License applies. Check COPYING for details.
 *
 * @brief  example unittest for the test object example in test_object.c
 */

/**
 * @addtogroup Example_Unittest
 * @{
 */

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "test_object.h"   /* object to test */

#include "unittest.h"      /* unittest framework access */


/**
 * @brief main function for Unittest example
 * @param argc
 * @param argv
 * @return
 */
int main(int argc, char **argv)
{
    int16_t a,b;

    UT_ENABLE_HTML();

    UT_BEGIN("Example for ASM Software Unittest Framework");

    UT_TESTCASE("Hand picked examples");
    UT_DESCRIPTION("Some arbitrary value pairs are tested.");

    UT_PRECONDITION(a = 1);
    UT_PRECONDITION(b = 2);

    UT_COMMENT("b > a");
    UT_TEST(Max(a,b) == 2);

    UT_PRECONDITION(a = 100);
    UT_PRECONDITION(b = 20);
    UT_COMMENT("a > b");
    UT_TEST(Max(a,b) == 100);

    for(b = -32000; b < 32000; b += 1600)
    {
        char comment_str[50];

        UT_TESTCASE("Example with fix a and moving b");

        UT_PRECONDITION(a = 5200);

        sprintf(comment_str,"b = %d", b);

        UT_PRECONDITION_STR(comment_str);

        if(a < b)
        {
            UT_TEST(Max(a,b) == b);
        }
        else
        {
            UT_TEST(Max(a,b) == a);
        }

    }

    UT_END();

    #ifdef GCOV_ENABLED
        return 0;
    #else
        return UT_Result;
    #endif
}

/** @} */
