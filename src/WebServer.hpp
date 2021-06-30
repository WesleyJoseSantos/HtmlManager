/**
 * @file WebServer.hpp
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

class WebServer : public AsyncWebHandler
{
private:
    String dataToReceive;
    String dataToSend;
    String homepage;
    bool dataReceived;

    String getContentType(String url){
        if(url.endsWith("jso")){
            return "application/json";
        }else if(url.endsWith("css")){
            return "text/css";
        }else if(url.endsWith("png")){
            return "image/png";
        }else if(url.endsWith("ico")){
            return "image/x-icon";
        }else if(url.endsWith("js")){
            return "application/javascript";
        }else{
            return "text/html";
        }
    }

public:
    WebServer() {
        homepage = "/home.htm";
    }
    
    virtual ~WebServer() {}

    bool canHandle(AsyncWebServerRequest *request){
        return true;
    }

    void handleRequest(AsyncWebServerRequest *request){
        //Serial.print("Request Received ");
        //Serial.println(request->methodToString());
        //Serial.println(request->url());

        if(request->method() == 1){
            if(request->hasParam("data")){
                String type = getContentType(request->url());
                request->send(200, type, dataToSend);
            }else{
                if(request->url() == "/"){
                    request->send(SPIFFS, homepage, "text/html");
                }else{
                    String type = getContentType(request->url());
                    request->send(SPIFFS, request->url(), type);
                }
            }
        }
        
        if(request->method() == 2){
            if(request->hasParam("data")){
                dataToReceive = request->getParam("data")->value();
                dataReceived = true;
                //Serial.println(data);
                request->send(200, "text/html", "OK");
            }
        }
    }

    void setHomePage(String homepage){
        this->homepage = homepage;
    }

    String getData(){
        dataReceived = false;
        return dataToReceive;
    }

    bool dataAvailable(){
        return dataReceived;
    }

    void setData(String data){
        this->dataToSend = data;
    }
};