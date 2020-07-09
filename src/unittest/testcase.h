/*
 * NodeFlight - platform for embedded control systems
 * Copyright (C) 2020  Max Sikström
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

#pragma once

#include "lib/varsection.h"
#include <check.h>

/**
 * Define a unit test case for nodeflight
 *
 * Same as libcheck START_TEST (no END_TEST actually needed), but also registers the test case for execution
 */
#define NF_TESTCASE(_name) \
    static void _name ## _fn(int _i CK_ATTRIBUTE_UNUSED); \
    static const TTest _name ## _ttest = {""# _name, _name ## _fn, __FILE__, __LINE__}; \
    static const TTest *_name VARSECTION_ATTR(nf_testcase) = &_name ## _ttest; \
    static void _name ## _fn(int _i CK_ATTRIBUTE_UNUSED)
