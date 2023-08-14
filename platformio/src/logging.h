/*
 *   This file belongs to the Project ESP-ToDoViewer-MSToDo
 *   Copyright (c) 2023 Peter Tißen
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.

 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.

 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#pragma once

#include <Arduino.h>
#include "config/config.h"

#define logE if(LOG_ERROR)     Serial
#define logW if(LOG_WARNING)   Serial
#define logI if(LOG_INFO)      Serial
#define logD if(LOG_DEBUG)     Serial

#if LOG_SERIAL == true || LOG_ERROR == true || LOG_WARNING == true || LOG_INFO == true || LOG_DEBUG == true
    #define LOG_ISACTIVATED true
    #define logSerial Serial
#endif

void logInit();

