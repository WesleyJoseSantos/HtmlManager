/**
 * @file WiFiManagerData.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "ArduinoJson.h"

#define TO_KEY(x) #x
#define WIFI_FILE "/wifi.json"
#define WIFI_FILE_BKP "/wifibkp.json"

class WiFiManagerData
{
private:
    const char *ssid;
    const char *pass;
public:
    WiFiManagerData() {}
    ~WiFiManagerData() {}

    void save(){
        String json = this->toJson();
        File file = SPIFFS.open(WIFI_FILE, "w+");
        file.print(json);
        file.close();
    }

    bool load(){
        if(SPIFFS.exists(WIFI_FILE)){
                File file = SPIFFS.open(WIFI_FILE, "r");
                if(!file){
                String json = file.readString();

                WiFiManagerData data = WiFiManagerData::fromJson(json);
                this->ssid = data.ssid;
                this->pass = data.pass;
                file.close();
                return true;
            }
        }
        return false;
    }

    void reset(){
        SPIFFS.rename(WIFI_FILE, WIFI_FILE_BKP);
    }

    void restore(){
        SPIFFS.rename(WIFI_FILE_BKP, WIFI_FILE);

    }

    static WiFiManagerData fromJson(String serializedJson){
        StaticJsonDocument<256> jsonDoc;
        WiFiManagerData data;
        deserializeJson(jsonDoc, serializedJson);

        data.ssid = jsonDoc[TO_KEY(ssid)];
        data.pass = jsonDoc[TO_KEY(pass)];

        return data;
    }

    String toJson(){
        StaticJsonDocument<256> jsonDoc;
        String serializedJson;

        jsonDoc[TO_KEY(ssid)] = ssid;
        jsonDoc[TO_KEY(pass)] = pass; 

        serializeJson(jsonDoc, serializedJson);
        return serializedJson;
    }

    String getSsid(){
        return ssid;
    }

    String getPass(){
        return pass;
    }
};