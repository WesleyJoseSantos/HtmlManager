/**
 * @file SimpleWiFiManager.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "FS.h"
#include <DNSServer.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#include "SPIFFS.h"
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include "ESPAsyncWebServer.h"
#include "WebServer.hpp"
#include "WiFiManagerData.hpp"

#define HOME_PAGE "/home.htm"

class SimpleWiFiManager
{
private:
    AsyncWebServer server;
    WebServer webServerWiFiManager;
    WiFiManagerData data;
    DNSServer dnsServer;
    bool provisioned;

    bool connect(){
        WiFi.begin(data.getSsid(), data.getPass());
        Serial.print("Connecting to ");
        Serial.print(data.getSsid()); Serial.println(" ...");
        int i = 0;
        while (WiFi.status() != WL_CONNECTED) {
            delay(1000);
            Serial.print(++i); Serial.print(' ');
            if(i >= 20){
                Serial.println("Connection failed!");
                return false;
            }
        }
        provisioned = true;
        Serial.println("Connection sucess!");
        return true;
    }

public:
    SimpleWiFiManager():server(80) {}

    void begin(){
        Serial.begin(74400);
        if(!SPIFFS.begin()){
            Serial.println("SPIFFS Mount Failed!");
        }
        webServerWiFiManager.setHomePage(HOME_PAGE);
        if(data.load()){
            Serial.println("Using previous credentials.");
            if(!connect()){
                Serial.println("Credentials refused.");
                ESP.reset();
            }
            dnsServer.stop();
            dnsServer.start(54, "devlocalcontrol.com", WiFi.localIP());
        }else{
            WiFi.softAP("WiFi Manager");
            dnsServer.start(53, "*", WiFi.softAPIP());
            server.addHandler(&webServerWiFiManager);
            server.begin();
            Serial.printf("Started server at %s\n", WiFi.softAPIP().toString().c_str());
        }
    }

    void task(){
        if(!provisioned){
            dnsServer.processNextRequest();
            if(webServerWiFiManager.dataAvailable()){
                String json = webServerWiFiManager.getData();
                data.fromJson(json);
                webServerWiFiManager.setData("connecting");
                if(connect()){
                    webServerWiFiManager.setData(WiFi.localIP().toString());
                    data.save();
                }else{
                    webServerWiFiManager.setData("Connection failed!");
                }
            }
        }
    }

    void reset(){
        data.reset();
    }

    void restore(){
        data.restore();
    }
};