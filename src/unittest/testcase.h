#pragma once

#include "lib/varsection.h"

typedef void (*testcase_t)(
    void);

#define TESTCASE(_name) \
    static void testcase_ ## _name(void); \
    static const testcase_t testcase_ptr_ ## _name VARSECTION_ATTR(nf_testcase) = testcase_ ## _name; \
    void testcase_ ## _name(void) \

