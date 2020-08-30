/**
 * @file   unittest.c
 *
 * ASM Software Unittest Framework
 * (c) 2016 by Matthias Arndt <marndt@asmsoftware.de>
 *
 * The MIT License applies. Check COPYING for details.
 *
 * @brief   framework implementation
 * @details The functions are meant to be called through the associated macros.
 */

/**
 * @addtogroup ASMSoftware_Unittest
 * @{
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

/*-----------------------------------------------
 * internal variables
 *-----------------------------------------------
 */

/**
 * @brief internal unittest state
 */
typedef struct
{
    int16_t test_nr;   /*!< number of testcases for testcase headlines */
    int16_t passed;    /*!< number of passed tests */
    int16_t failed;    /*!< number of failed tests */
    char comment[512]; /*!< temporary storage for testcase comment */
    char * cssname;     /*!< CSS styesheet reference, not outputted if NULL */
    bool enable_html;  /*!< enables HTML output */
    bool html_testcase;     /*!< testcase started. This closes an unfinished list on next testcase or footer output */
} Unittest;

static Unittest UT;  /*!< tracks the internal unittest state */

static time_t rawtime;        /*!< internal variable to create time stamps */
static struct tm * timeinfo;  /*!< decoding information for timestamp */

/*-----------------------------------------------
 * public variables
 *-----------------------------------------------
 */

int UT_Result = 0;

/**
 * @brief UT_Begin is an internal function
 * @see   UT_BEGIN
 * @param headline
 * @param filename
 */
void UT_Begin(const char *headline, const char *filename)
{
    UT.passed = 0;
    UT.failed = 0;
    UT.test_nr = 1;

    /* get current date and time */
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );

    if(UT.enable_html == true)
    {
        /* print header for HTML */
        printf("<!doctype html>\n");
        printf("<html lang=\"en\">\n");
        printf("<head>\n");
        printf("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n");
        printf("<title>Unittest report for &quot;%s&quot;</title>\n", headline);
        if(UT.cssname != NULL)
        {
            printf("<link rel=\"stylesheet\" type=\"text/css\" href=\"%s\">\n", UT.cssname);
        }
        printf("</head>\n");
        printf("<body>\n");
        printf("<h1>Unittest report for &quot;%s&quot;</h1>\n", headline);

        printf("<dl class=\"header\">\n");
        printf("<dt>FILE:</dt><dd>%s</dd>\n", filename);
        printf("<dt>DATE:</dt><dd>%s</dd>\n", asctime(timeinfo));
        printf("</dl>\n");
        printf("<hr>\n");
    }
    else
    {
        /* print header for text */
        printf("----  Unittest  -----------------------------------------------\n");
        printf("FILE: %s\n", filename);
        printf("DATE: %s", asctime(timeinfo));
        printf("---------------------------------------------------------------\n");
        printf("%s\n", headline);
        printf("---------------------------------------------------------------\n");
        printf("\n");
    }
}

/**
 * @brief UT_End is an internal function
 * @see   UT_END
 */
void UT_End(void)
{
    /* compute success rate: */
    float rate = 100.0f;

    if(UT.failed > 0)
    {
        const float sum_tests = (float)UT.passed + (float)UT.failed;
        rate = ((float)UT.passed / sum_tests) * 100.0f;
    }

    if(UT.enable_html == true)
    {
        if(UT.html_testcase == true)
        {
            UT.html_testcase = false;
            printf("</dl>\n");
        }
        printf("<hr>\n");
        printf("<h2>Summary</h2>\n");

        printf("<dl>\n");
        printf("<dt>Passed tests</dt><dd>%d</dd>\n", UT.passed);
        printf("<dt>Failed tests</dt><dd>%d</dd>\n", UT.failed);
        printf("<dt>Success rate</dt><dd>%3.2f%%</dd>\n", rate);
        printf("</dl>\n");
        printf("<hr>\n");

        printf("<p><em>\n");
    }
    else
    {
        /* print footer for text */
        printf("---------------------------------------------------------------\n");
        printf("Passed tests ....: %d\n", UT.passed);
        printf("Failed tests ....: %d\n", UT.failed);
        printf("Success rate ....: %3.2f%%\n", rate);
        printf("---------------------------------------------------------------\n");
    }

    if(UT.failed != 0)
    {
        printf("Test result NOT OK!\n");
        UT_Result += UT.failed;          /* overall sum of failed testcases */
    }
    else
    {
        printf("Test result OK!\n");
    }

    if(UT.enable_html == true)
    {
        printf("</em></p>\n");
        printf("</body>\n</html>\n");
    }

    return;
}

/**
 * @brief UT_Testcase is an internal function
 * @see   UT_TESTCASE
 * @param headline
 */
void UT_Testcase(const char *headline)
{
    if(UT.enable_html == true)
    {
        if(UT.html_testcase == true)
        {
            UT.html_testcase = false;
            printf("</dl>\n");
        }

        printf("<h2>%d. %s</h2>\n",UT.test_nr, headline);
    }
    else
    {
        printf("== %d. %s\n\n", UT.test_nr, headline);
    }
    UT.test_nr++;
    return;
}

/**
 * @brief UT_Description is an internal function
 * @see UT_DESCRIPTION
 * @param desc
 */
void UT_Description(const char *desc)
{
    if(UT.enable_html == true)
    {
        printf("<p>\n");
    }

    printf("%s\n\n", desc);
    return;
}

/**
 * @brief UT_Precondition is an internal function
 * @see UT_PRECONDITION
 * @param cond_desc
 */
void UT_Precondition(const char *cond_desc)
{
    if(UT.enable_html == true)
    {
        if(UT.html_testcase == false)
        {
            UT.html_testcase = true;
            printf("<dl>\n");
        }
        printf("<dt>Precondition</dt><dd>%s</dd>\n", cond_desc);
    }
    else
    {
        printf("PRECONDITION: %s\n", cond_desc);
    }
    return;
}

/**
 * @brief UT_Test is an internal function
 * @see UT_TEST
 * @param test_cond
 * @param cond_desc
 */
void UT_Test(const bool test_cond, const char *cond_desc)
{     
    if(UT.enable_html == true)
    {
        if(UT.html_testcase == false)
        {
            UT.html_testcase = true;
            printf("<dl>\n");
        }
        printf("<dt>TEST: %s</dt><dd>", cond_desc);
    }
    else
    {
        printf("TEST: %s   ", cond_desc);
    }

    if(test_cond == true)
    {
        UT.passed++;
        printf("passed\n");
    }
    else
    {
        UT.failed++;
        printf("failed\n");
    }

    if(UT.enable_html == true)
    {
        printf("</dd>\n");
    }

    if(strlen(UT.comment) > 0)
    {
        if(UT.enable_html == true)
        {
            printf("<dd>&quot;%s&quot;</dd>\n", UT.comment);
        }
        else
        {
            printf("\"%s\"\n\n", UT.comment);
        }
        memset(UT.comment, '\0',sizeof(UT.comment) ); // empty comment after processing
    }

    printf("\n");

    return;
}


/**
 * @brief UT_SetComment is an internal function
 * @see UT_COMMENT
 * @param comment
 */
void UT_SetComment(const char *comment)
{
    size_t len_comment = strlen(comment);

    if(sizeof(UT.comment) < len_comment)
    {
        len_comment = sizeof(UT.comment);
    }

    memset(UT.comment, '\0',sizeof(UT.comment) ); // empty comment
    strncpy(UT.comment, comment, len_comment);  // copy string

    return;
}

/**
 * @brief UT_EnableHtml is an internal function
 * @see UT_ENABLE_HTML
 * @see UT_ENABLE_HTML_WITH_CSS
 * @param cssname
 */
void UT_EnableHtml(char *cssname)
{
    UT.cssname = cssname;
    UT.enable_html = true;
}

/** @} */
