/*
 * IOT Screamer - keeps sending pre-defined message to your telegram after power on 
 */

#define WIFI_SSID "SSID"
#define WIFI_PASS "PASS"
#define TELEGRAM_API_KEY "API_KEY"
#define CHAT_ID "<<chat_id_number>>"
#define MESSAGE_TEXT "%2aWater+Water+Water%21%21%21%2a%0d%0aThere+is+water+detected+under+the+stairs%21%21%21" // Using Telegram's Markdown syntax
#define REPEAT_INTERVAL 600 // seconds  

//#define BLINK 1

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClientSecureBearSSL.h>
// Fingerprint for demo URL, expires on June 2, 2019, needs to be updated well before this date

ESP8266WiFiMulti WiFiMulti;

#define TELEGRAM_API_BASE_URL "https://api.telegram.org/bot" TELEGRAM_API_KEY
#define TELEGRAM_SEND_MESSAGE_BASE_URL TELEGRAM_API_BASE_URL "/sendMessage?chat_id=" CHAT_ID 
#define TELEGRAM_WATER_WATER_WATER_MESSAGE_URL TELEGRAM_SEND_MESSAGE_BASE_URL "&parse_mode=Markdown&text=" MESSAGE_TEXT 

void setup() 
{
#ifdef BLINK
  pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
#else 
  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();
#endif 

  for (uint8_t t = 4; t > 0; t--) 
  {
#ifndef BLINK
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
#endif
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);
}

void loop() 
{
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) 
  {
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    client->setInsecure();

    HTTPClient https;

#ifndef BLINK
    Serial.print("[HTTPS] begin...\n");
#endif
    if (https.begin(*client, TELEGRAM_WATER_WATER_WATER_MESSAGE_URL)) 
    {

#ifndef BLINK
      Serial.print("[HTTPS] GET...\n");
#endif
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) 
      {
        // HTTP header has been send and Server response header has been handled
#ifndef BLINK
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);
#endif

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) 
        {
          String payload = https.getString();
#ifndef BLINK
          Serial.println(payload);
#endif
        }
      } 
      else 
      {
#ifndef BLINK
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
#endif
      }

      https.end();
    } 
    else 
    {
#ifndef BLINK
      Serial.printf("[HTTPS] Unable to connect\n");
#endif
    }
  }

#ifndef BLINK
  Serial.println("Wait 10m before next round...");
  delay(REPEAT_INTERVAL * 1000);
#else 
  for (int i = 0; i < REPEAT_INTERVAL / 3; ++ i)
  {
    //digitalWrite(LED_BUILTIN, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is active low on the ESP-01)
    delay(1000);                      // Wait for a second
    //digitalWrite(LED_BUILTIN, HIGH);  // Turn the LED off by making the voltage HIGH
    delay(2000);                      // Wait for two seconds (to demonstrate the active low LED)  
  }
#endif 
}
