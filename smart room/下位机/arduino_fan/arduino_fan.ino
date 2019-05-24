
#include <Adafruit_BMP085.h>
#include <ESP8266WiFi.h>
#include<PubSubClient.h>
#include<String.h>
#include <Wire.h>
#include <SFE_MicroOLED.h>
#include <WEMOS_SHT3X.h>
#include <EEPROM.h>
ADC_MODE(ADC_VCC);
#define PIN_RESET 255  //
#define DC_JUMPER 0  // I2C Addres: 0 - 0x3C, 1 - 0x3D
void(* resetFunc) (void) = 0;
//const char*mqttServer = "m14.cloudmqtt.com";
//const int mqttPort = 13713;
//const char*mqttUser = "dmrszgvw";
//const char*mqttPassword = "iVfd7J2l4ZZ0";
const char*mqttServer = "47.102.159.197";
const int mqttPort = 1883;
const char*mqttUser = "admin";
const char*mqttPassword = "public";
char temp_data[6];
char hum_data[6];
char alt_data[7];
char vol_data[6];
String mqtt_reciver = "";
char* LED_STATUS = "";
String LED_ON = "LED_ON";
String LED_OFF = "LED_OFF";
String GET_TEMP = "GET_TEMP";
String GET_HUM = "GET_HUM";
String GET_ALT = "GET_ALT";
String GET_VOL = "GET_VOL";
String SW_ON = "SW_ON";
String SW_OFF = "SW_OFF";

int sw_statu = 0;
char* fan_statu="0";
static int count = 0;

void oled_display_string(char*a);
int Count_Num(double a);   
WiFiClient espClient;

PubSubClient client(espClient);
MicroOLED oled(PIN_RESET, DC_JUMPER);
SHT3X sht30(0x45);
Adafruit_BMP085 bmp;

void callback(char*topic, byte* payload, unsigned int length)
{

  mqtt_reciver = "";
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    mqtt_reciver += (char)payload[i];
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("-----------------------");
  if (mqtt_reciver.equals(LED_ON))
  {

  }
  if (mqtt_reciver.equals(LED_OFF))
  {
   
  }
   if (mqtt_reciver.equals(SW_OFF))
  {
   
   
  }
  if (mqtt_reciver.equals("0"))
  {
    fan_statu="0";
    digitalWrite(D1, LOW);
    delay(10);
    digitalWrite(D1, HIGH);
    client.publish("esp8266_fan/from/status","0"); 
  }
  if (mqtt_reciver.equals("1"))
  {
    fan_statu="1";
    digitalWrite(D2, LOW);
    delay(10);
    digitalWrite(D2, HIGH);
    client.publish("esp8266_fan/from/status","1"); 
  }
  if (mqtt_reciver.equals("2"))
  {
    fan_statu="2";
    digitalWrite(D3, LOW);
    delay(10);
    digitalWrite(D3, HIGH);
    client.publish("esp8266_fan/from/status","2"); 
  }
  if (mqtt_reciver.equals("3"))
  {
    fan_statu="3";
    digitalWrite(D4, LOW);
    delay(10);
    digitalWrite(D4, HIGH);
    client.publish("esp8266_fan/from/status","3"); 
  }
 if (mqtt_reciver.equals("REQUEST"))
  {
   client.publish("esp8266_fan/from/status",fan_statu);
  }
  
}




bool autoConfig()
{

  for (int i = 0; i < 10; i++)
  {
    int wstatus = WiFi.status();
    if (wstatus == WL_CONNECTED)
    {
      Serial.println("AutoConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());

      return true;
      //break;
    }
    else
    {

      Serial.print("AutoConfig Waiting......");
      Serial.println(wstatus);
      
    }
  }
  Serial.println("AutoConfig Faild!" );
  return false;

}



void smartConfig()
{
  WiFi.mode(WIFI_STA);
  Serial.println("\r\nWait for Smartconfig");
  WiFi.beginSmartConfig();
  while (1)
  {

    Serial.print(".");
    if (WiFi.smartConfigDone())
    {

      Serial.println("SmartConfig Success");
      Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
      Serial.printf("PSW:%s\r\n", WiFi.psk().c_str());
      WiFi.setAutoConnect(true);  // 设置自动连接
      break;
    }
    delay(500); // 这个地方一定要加延时，否则极易崩溃重启
   
  }
}

void Switch_key()
{


}





int Count_Num(double a)    //判断整数位数
{
      if(int(a/1000))
      {
        return 4;
      }else if(int(a/100))
      {
        return 3;
        }else if(int(a/10))
        {
          return 2;
          }else if(int(a/1))
          {
            return 1;
            }else
            {
              return 0;
              }
}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client_fan",mqttUser, mqttPassword)) {
      Serial.println("connected");
      digitalWrite(D1, LOW);
      delay(10);
      digitalWrite(D1, HIGH);
      // Once connected, publish an announcement...
      client.publish("esp8266_fan/from/status", "ok");
    
      client.subscribe("esp8266_fan/to");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
      if (WiFi.status() != WL_CONNECTED)
      {
        break;
        }
    }
}
}


void setup()
{

  Serial.begin(115200);
 
  pinMode(D1, OUTPUT);
  pinMode(D2, OUTPUT);
  pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  digitalWrite(D1, HIGH);
  digitalWrite(D2, HIGH);
  digitalWrite(D3, HIGH);
  digitalWrite(D4, HIGH);
  digitalWrite(D2, LOW);
  delay(50);
  digitalWrite(D2, HIGH);
 // attachInterrupt(digitalPinToInterrupt(D3), Switch_key , FALLING);
  if (!autoConfig())
  {
    Serial.println("Start module");
    smartConfig();
  }
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);
  while (!client.connected() && (WiFi.status() == WL_CONNECTED))
  {
    Serial.println("Connecting to MQTT...");
    if (client.connect("ESP8266Client_fan", mqttUser, mqttPassword )) {
      Serial.println("connected");
      digitalWrite(D1, LOW);
      delay(10);
      digitalWrite(D1, HIGH);
      
    } else {
      Serial.println("failed with state ");
      Serial.print(client.state());
      digitalWrite(D3, LOW);
      delay(10);
      digitalWrite(D3, HIGH);
      delay(2000);
    }
  }
  client.subscribe("esp8266_fan/to");
  client.publish("esp8266_fan/from/status", "ok");

}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
 
  if (WiFi.status() != WL_CONNECTED)
  {
     
      ESP.restart();
   

  }
  

}
