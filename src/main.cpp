#include <Arduino.h>
#include <WiFi.h>
#include <Esp32MQTTClient.h>

#define RXD 16
#define TXD 17
/* ssid password of the target WiFi access point */
const char * ssid = "iPhone";
const char * password = "TUVIOT123";

/* Connection String of the device to connect to target IoT Hub */
static const char * connectionString = "HostName=C-Test-Hub.azure-devices.net;DeviceId=WROOM;SharedAccessKey=weiihCmXhP3n0EKhncuIVgodKiW0dNw01uWhPoiKO1M=";

char msg;
char * iotMsg;

static bool isHubOn = false;

void setup() {
  /* Initializing Serial 0 for communication with the host machine and device */
  Serial.begin(115200);
  /* Initializing Serial 2 for UART communication */
  Serial2.begin(38400, SERIAL_8N1, RXD, TXD);

  Serial.println("");
  Serial.println("Starting WiFi connection");
  delay(100);

  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connection was successful");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if(!Esp32MQTTClient_Init((const uint8_t*)connectionString)){
    isHubOn = false;
    Serial.println("Initializing IoT Hub failed.");
    return;
  }

  Serial.println("Sending Events");
  isHubOn = true;
}

void loop() {
  if(isHubOn){
    /* Sending message to target IoT Hub */
    char buff[128];

    msg = char(Serial2.read());
    iotMsg = &msg;

    snprintf(buff, 128, "{Message: %s}", iotMsg);

    /* Check if message was successfully sent of not */
    if(Esp32MQTTClient_SendEvent(buff)){
      Serial.println("Message sent successfully");
    }
    else{
      Serial.println("Failed to send message...");
    }

    delay(1000);
  }
}