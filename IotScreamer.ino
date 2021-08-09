/*
 * IOT Screamer - keeps sending pre-defined message to your telegram after power on 
 */

#define WIFI_SSID "SSID"
#define WIFI_PASS "WiFiPassword"
#define TELEGRAM_API_KEY "--TG-BOT-API-KEY--"
#define CHAT_ID "--tg-user-id--"
#define MESSAGE_TEXT "%2aWater+Water+Water%21%21%21%2a%0d%0aThere+is+water+detected+under+the+stairs%21%21%21" // Using Telegram's Markdown syntax
#define REPEAT_INTERVAL 600 // seconds  

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>

ESP8266WiFiMulti WiFiMulti;

#define TELEGRAM_API_BASE_URL "https://api.telegram.org/bot" TELEGRAM_API_KEY
#define TELEGRAM_SEND_MESSAGE_BASE_URL TELEGRAM_API_BASE_URL "/sendMessage?chat_id=" CHAT_ID 
#define TELEGRAM_WATER_WATER_WATER_MESSAGE_URL TELEGRAM_SEND_MESSAGE_BASE_URL "&parse_mode=Markdown&text=" MESSAGE_TEXT 

void setup() 
{
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) 
  {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);
}

void loop() 
{
  // wait for WiFi connection

  switch (WiFiMulti.run())
  {
    case WL_IDLE_STATUS:
      Serial.print("WL_IDLE_STATUS...\n");
      delay(1000);
      return;
      
    case WL_NO_SSID_AVAIL:
      Serial.print("WL_NO_SSID_AVAIL...\n");
      delay(1000);
      return;
      
    case WL_SCAN_COMPLETED:
      Serial.print("WL_SCAN_COMPLETED...\n");
      delay(1000);
      return;
      
    case WL_CONNECTED:
      Serial.print("WL_CONNECTED\n");
      break;
      
    case WL_CONNECT_FAILED:
      Serial.print("WL_CONNECT_FAILED\n");
      delay(1000);
      return;
      
    case WL_CONNECTION_LOST:
      Serial.print("WL_CONNECTION_LOST\n");
      delay(1000);
      return;
      
    case WL_DISCONNECTED:
      Serial.print("WL_DISCONNECTED\n");
      delay(1000);
      return;
      
    default:
      Serial.print("WiFi: unknown state\n");
      delay(1000);
      return;
  }
  
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    client->setInsecure();

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");

    if (https.begin(*client, TELEGRAM_WATER_WATER_WATER_MESSAGE_URL)) 
    {
      Serial.print("[HTTPS] GET...\n");

      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) 
      {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
        {
          String payload = https.getString();
          Serial.println(payload);
        }
      } 
      else 
      {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }

      https.end();
    } 
    else 
    {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
  }
  else
  {
    Serial.print("WiFi is not connected ...\n");
  }

  for (int i = 0; i < REPEAT_INTERVAL / 3; ++ i)
  {
    delay(3000);
    Serial.printf("Sleeping... %d\n", i);
  }
}
