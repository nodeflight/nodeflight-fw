/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2023  Max Sikström
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include "lib/varsection.h"
#include "unittest/testcase.h"

VARSECTION_ACCESS(TTest *, nf_testcase)

int main(
    void)
{
    Suite *s;
    TCase *tc;
    int i;
    int number_failed;
    SRunner *sr;

    /* Create suite */
    s = suite_create("NodeFlight");
    tc = tcase_create("unit tests");
    suite_add_tcase(s, tc);
    for (i = 0; &__nf_testcase_start[i] < __nf_testcase_end; i++) {
        tcase_add_test(tc, __nf_testcase_start[i]);
    }

    /* Execute tests */
    sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (number_failed == 0) ? 0 : 1;
}
