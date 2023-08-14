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
#include "App.h"

#include "logging.h"
#include "config/config.h"
#include "config/secrets.h"
#include "globalvar.h"
#include <ctime>

void App::ini(){
    logInit();

    // Check System Status
    if(!isBootReasonOk()){
        activateDeepSleep(true,m_timeHandler);
        return;
    }
   
    // Battery
    m_battery.ini();
    m_battery.update();
    if(m_battery.isBatteryLow() == true) {
        if(m_battery.wasBatteryLow() == false){
            m_displayHandler.drawError(DisplayHandler::ERR_BATTERY);
        }
        activateDeepSleep(true,m_timeHandler);
        return;
    }
  
    // WIFI
    m_wifiHandler.ini();
    m_wifiHandler.update();
    if(!m_wifiHandler.isConnected()){
        m_wifiHandler.kill();
        m_displayHandler.drawError(DisplayHandler::ERR_WIFI);
        activateDeepSleep(false,m_timeHandler);
        return;
    }

    // Time
    m_timeHandler.ini();
    m_timeHandler.update();
    if(!m_timeHandler.isValid()){
        m_wifiHandler.kill();
        m_displayHandler.drawError(DisplayHandler::ERR_TIME);
        activateDeepSleep(false,m_timeHandler);
        return;
    }

    // ToDo
    m_todo.update();
    if(m_todo.isError()){
        m_wifiHandler.kill();
        m_displayHandler.drawError(DisplayHandler::ERR_API);
        activateDeepSleep(false,m_timeHandler);
        return;
    }

    GL_WIFI_DBM = m_wifiHandler.rssi();
    GL_BAT_PERCENTAGE = m_battery.percent();
    GL_BAT_VOLTAGE = m_battery.voltage();
    GL_TIME_LASTUPDATE = m_timeHandler.lastUpdate();

    m_wifiHandler.kill();

    m_displayHandler.update();
    activateDeepSleep(false,m_timeHandler);
}

String timeToString(time_t timeValue) {
  struct tm *tmInfo;
  tmInfo = localtime(&timeValue);

  char timeStr[25]; 
  strftime(timeStr, sizeof(timeStr), "%F %T", tmInfo);

  return String(timeStr);
}

void App::activateDeepSleep(bool forever, TimeHandler &t) {
    logD.println("\n------------ DeepSleep ----------");
    delay(1);

    if (forever) {
        // Sleep forever
        logI.print("  Activate Deepsleep: forever");
        esp_sleep_enable_timer_wakeup(0);
        esp_deep_sleep_start();
        return;
    }

    uint64_t sleepTime = 3600;

    if(t.isValid()){
        tm * ts= t.lastUpdate();
    
        if(ts->tm_hour >= DEEPSLEPP_START_H || ts->tm_hour < DEEPSLEPP_END_H){
            tm zielZeit = *ts;  
            zielZeit.tm_hour = DEEPSLEPP_END_H;
            zielZeit.tm_min = 0;      
            zielZeit.tm_sec = 0;     

            time_t wakeTime = mktime(&zielZeit); 

            if(ts->tm_hour >= DEEPSLEPP_START_H){
                wakeTime += (24*60*60);
            }

            time_t currTime = mktime(ts);
            logI.printf("  Current time    : %ld : %s\n", currTime, timeToString(currTime).c_str());
            logI.printf("  Wake up time    : %ld : %s\n", wakeTime, timeToString(wakeTime).c_str());
            
            sleepTime = (uint64_t) difftime(wakeTime, currTime);
        }
    }

    logI.print("  Activate Deepsleep (s): " + String(sleepTime) + "\n");

    esp_sleep_enable_timer_wakeup(sleepTime * 1000000);
    esp_deep_sleep_start();
}


bool App::isBootReasonOk(){
     // Lese den Grund für den letzten Neustart aus
    logD.println("\n------------ System ----------");
    ++SYS_BOOT_COUNT;
    logD.println("  Boot number: " + String(SYS_BOOT_COUNT));

    esp_reset_reason_t reason = esp_reset_reason();
    switch (reason) {
    case ESP_RST_PANIC:
        logW.println("  Software-Reset due to Exception/Panic");
        return false;
    case ESP_RST_INT_WDT:
        logW.println("  Reset due to Interrupt Watchdog");
        return false;
    case ESP_RST_TASK_WDT:
        logW.println("  Reset due to Task Watchdog");
        return false;
    case ESP_RST_WDT:
        logW.println("  Reset due to other Watchdogs");
        return false;

    case ESP_RST_POWERON:
        logD.println("  PowerOn-Reset");
        break;
    case ESP_RST_SW:
        logD.println("  Software Reset");
        break;
    case ESP_RST_DEEPSLEEP:
        logD.println("  Reset after exiting Deep Sleep mode");
        break;
    case ESP_RST_BROWNOUT:
        logD.println("  Brownout Reset");
        break;
    case ESP_RST_SDIO:
        logD.println("  Reset due to SDIO");
        break;
    case ESP_RST_UNKNOWN:
    default:
        logW.println("  Unknown Reset Reason");
        break;
    }

    return true;
}