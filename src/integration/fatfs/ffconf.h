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

/* For more information, see FatFs documentation */

#define FFCONF_DEF      86606   /* Revision ID */

/* fatfs functionality */

#define FF_FS_READONLY          0
#define FF_FS_MINIMIZE          0
#define FF_USE_STRFUNC          1
#define FF_USE_FIND             0
#define FF_USE_MKFS             0
#define FF_USE_FASTSEEK         0
#define FF_USE_EXPAND           0
#define FF_USE_CHMOD            0
#define FF_USE_LABEL            0
#define FF_USE_FORWARD          0

/* String conversion */

#define FF_CODE_PAGE            850

#define FF_USE_LFN              2
#define FF_MAX_LFN              255
#define FF_LFN_UNICODE          2
#define FF_LFN_BUF              255
#define FF_SFN_BUF              12

#define FF_STRF_ENCODE          0

#define FF_FS_RPATH             0

/* Volumes */

#define FF_VOLUMES              4
#define FF_STR_VOLUME_ID        2
#define FF_VOLUME_STRS          "int", "ext1", "ext2", "ext3"

/* Partitions/sectors */

#define FF_MULTI_PARTITION      0
#define FF_MIN_SS               512
#define FF_MAX_SS               512
#define FF_LBA64                0
#define FF_MIN_GPT              0x100000000
#define FF_USE_TRIM             0

/* System Configurations */

#define FF_FS_TINY              0
#define FF_FS_EXFAT             0

#define FF_FS_NORTC             1
#define FF_NORTC_MON            1
#define FF_NORTC_MDAY           1
#define FF_NORTC_YEAR           2020

#define FF_FS_NOFSINFO          0
#define FF_FS_LOCK              0

/* OS integration */

#include "FreeRTOS.h"
#include "semphr.h"
#define FF_FS_REENTRANT         1
#define FF_FS_TIMEOUT           1000
#define FF_SYNC_t               SemaphoreHandle_t
