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

#include "module/nfcp/nfcp.h"

extern const nfcp_cls_t nfcp_cls_cap;

#define NFCP_CLS_CAP                        1
#define NFCP_CLS_CAP_GET_INFO               0
#define NFCP_CLS_CAP_GET_RS                 1
#define NFCP_CLS_CAP_GET_PP                 2
#define NFCP_CLS_CAP_GET_MD                 3
#define NFCP_CLS_CAP_NUM_OPS                4
