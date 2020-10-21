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

#ifdef __MACH__

/* macos */

#define VARSECTION_ATTR(_SECTION) \
    __attribute__((used, section("__DATA,__" #_SECTION)))

#define VARSECTION_ACCESS(_VARTYPE, _SECTION) \
    extern _VARTYPE __ ## _SECTION ## _start[] __asm("section$start$__DATA$__" #_SECTION); \
    extern _VARTYPE __ ## _SECTION ## _end[]  __asm("section$end$__DATA$__" #_SECTION);

#else

/*
 * elf/gcc based
 *
 * Expects the section being included from the .ld script, and that there are two symbols defined for each section called:
 * __section_start
 * __section_end
 *
 * in the beginning and end of the sections
 */

#define VARSECTION_ATTR(_SECTION, _SYMNAME) \
    __attribute__ ((section("." #_SECTION "." #_SYMNAME), used))

#define VARSECTION_ACCESS(_VARTYPE, _SECTION) \
    extern const _VARTYPE __ ## _SECTION ## _start[]; \
    extern const _VARTYPE __ ## _SECTION ## _end[];

#endif
