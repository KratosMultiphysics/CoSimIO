/*   ______     _____ _           ________
    / ____/___ / ___/(_)___ ___  /  _/ __ |
   / /   / __ \\__ \/ / __ `__ \ / // / / /
  / /___/ /_/ /__/ / / / / / / // // /_/ /
  \____/\____/____/_/_/ /_/ /_/___/\____/
  Kratos CoSimulationApplication

  License:         BSD License, see license.txt

  Main authors:    Philipp Bucher (https://github.com/philbucher)
*/

/* this file contains some test-macros */

/* System includes */
#include <stdio.h>
#include <math.h>
#include <string.h>

/*
    Before the 23 standard, C and C++ handled unused arguments differently.
    C does not allow variable names to be omitted, but does not complain if
    the variable is unused. On the other hand, C++ allows variable names to
    be omitted, but will complain if it is not used when named.
    The "[[maybe_unused]]" attribute is a C23 extension, so it's not applicable
    here until support for earlier standards is dropped.
*/
#ifndef __cplusplus
    #define COSIMIO_MAYBE_UNUSED(variable_name) variable_name
#else
    #define COSIMIO_MAYBE_UNUSED(variable_name)
#endif

#define COSIMIO_CHECK_INT_EQUAL(a, b)                                \
    if (a != b) {                                                    \
        printf("in line %d: %d is not equal to %d", __LINE__, a, b); \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_DOUBLE_EQUAL(a, b)                             \
    if (fabs(a-b)>1e-15) {                                           \
        printf("in line %d: %f is not equal to %f", __LINE__, a, b); \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_STRING_EQUAL(a, b)                             \
    if (strcmp(a,b)) {                                               \
        printf("in line %d: %s is not equal to %s", __LINE__, a, b); \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_TRUE(a)                                        \
    if (a!=1) {                                                      \
        printf("in line %d: %d is not true!", __LINE__, a);          \
        return 1;                                                    \
    }

#define COSIMIO_CHECK_FALSE(a)                                       \
    if (a!=0) {                                                      \
        printf("in line %d: %d is not false!", __LINE__, a);         \
        return 1;                                                    \
    }
