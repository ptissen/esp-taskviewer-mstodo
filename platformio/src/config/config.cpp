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
#include "config.h"

const char* urlAuth = "https://login.microsoftonline.com/common/oauth2/v2.0/token";
const char* urlLists = "https://graph.microsoft.com/v1.0/me/todo/lists";
const char* urlTasks = "https://graph.microsoft.com/v1.0/me/todo/lists/";
const char* scope = "Tasks.Read Tasks.ReadWrite";
const char* grantType = "refresh_token";