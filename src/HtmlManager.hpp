/**
 * @file HtmlManager.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-15
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#pragma once

#include "ESPAsyncWebServer.h"
#include "FS.h"

#ifdef  ESP32
#include "SPIFFS.h"
#endif  //ESP32

class HtmlManager : public AsyncWebHandler
{
private:
    String data;
    String homepage;
    bool dataReceived;
public:
    HtmlManager() {}
    virtual ~HtmlManager() {}

    bool canHandle(AsyncWebServerRequest *request){
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request){
        Serial.print("Request Received ");
        Serial.println(request->method());

        if(request->method() == 1){
            if(request->hasParam("data")){
                Serial.println(data);
                request->send(200, "text/html", data);
            }else if(request->hasParam("screen")){
                String screen = request->getParam("screen")->value();
                request->send(SPIFFS, screen, "text/html");
                Serial.println(screen);
            }else{
                request->send(SPIFFS, homepage, "text/html");
                Serial.println(homepage);
            }
        }
        
        if(request->method() == 2){
            if(request->hasParam("data")){
                data = request->getParam("data")->value();
                dataReceived = true;
                Serial.println(data);
                request->send(200, "text/html", "OK");
            }
        }
    }

    void setHomePage(String homepage){
        this->homepage = homepage;
    }

    String getData(){
        dataReceived = false;
        return data;
    }

    bool dataAvailable(){
        return dataReceived;
    }

    void setData(String data){
        this->data = data;
    }
};