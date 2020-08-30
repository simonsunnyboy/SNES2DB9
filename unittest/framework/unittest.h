/**
 * @file   unittest.h
 *
 * ASM Software Unittest Framework
 * (c) 2016 by Matthias Arndt <marndt@asmsoftware.de>
 *
 * The MIT License applies. Check COPYING for details.
 *
 * @brief   API for unittest framework
 * @details The end user shall use the macros only. They wrap the functionality
 *          in a systematic way.
 */

/**
 * @addtogroup ASMSoftware_Unittest
 * @{
 */

#ifndef UNITTEST_H
    #define UNITTEST_H

    #include <stdint.h>
    #include <stdbool.h>

    /**
     * @brief return code to be used by the caller, 0 for success, number of overall failed test otherwise
     */
    extern int UT_Result;

    /*-----------------------------------------------
     * macros for use by the user
     *-----------------------------------------------
     */

    /**
     * @brief   starts a unittest
     * @details The originating filename, headline and current date of test execution are documented.
     *          The test must close with UT_END()
     * @param   headline
     */
    #define UT_BEGIN(headline)   UT_Begin(headline, __FILE__)

    /**
     * @brief   ends a unittest output with statistical output
     * @details After issuing UT_END, a different file/function may start another unittest with UT_BEGIN()
     */
    #define UT_END()             UT_End()

    /**
     * @brief   documents a testcase with a headline
     * @details All testcase headlines are numbered automatically by the framework. The numbering is reset with UT_BEGIN()
     * @param   tc_headline
     */
    #define UT_TESTCASE(tc_headline) UT_Testcase(tc_headline)

    /**
     * @brief documents a description for the last started testcase
     * @param text
     */
    #define UT_DESCRIPTION(text)  UT_Description(text)

    /**
     * @brief   executes and documents the given precondition
     * @details Use this to document simple stimuli to your test object such as setting a single variable
     * @param   precond
     */
    #define UT_PRECONDITION(precond) (precond); UT_Precondition(#precond)

    /**
     * @brief   documents any preconditions as a string
     * @details Use this to document stimuli to your test object that are more elaborate then setting a variable.
     * @warning This command does not execute any preconditions. For documentary purpose only!
     * @param   precond_str
     */
    #define UT_PRECONDITION_STR(precond_str)   UT_Precondition(precond_str)

    /**
     * @brief     evaluates the given test condition and counts success or failure
     * @details   The given condition is documented through a description.
     * @param     cond is a boolean test condition which should evaluate to true to pass the test
     */
    #define UT_TEST(cond)         UT_Test((cond), #cond)

    /**
     * @brief     sets a commentary string for the next UT_TEST() command
     * @param     comment
     */
    #define UT_COMMENT(comment)   UT_SetComment(comment)

    /**
     * @brief    enables HTML output
     * @details  This should be the first command issued to the framework if desired.
     */
    #define UT_ENABLE_HTML()      UT_EnableHtml(NULL)

    /**
     * @brief    enables HTML output and links with a given CSS stylesheet definition
     * @details  This should be the first command issued to the framework if desired.
     */
    #define UT_ENABLE_HTML_WITH_CSS(css)  UT_EnableHtml(css)

    /*-----------------------------------------------
     * internal definitions
     *-----------------------------------------------
     */

    void UT_Begin(const char *headline, const char *filename);
    void UT_End(void);
    void UT_Testcase(const char *headline);
    void UT_Description(const char *desc);
    void UT_Precondition(const char *cond_desc);
    void UT_Test(const bool test_cond, const char *cond_desc);
    void UT_SetComment(const char *comment);    
    void UT_EnableHtml(char *cssname);
#endif

/** @} */
