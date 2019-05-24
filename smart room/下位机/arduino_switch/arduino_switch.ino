
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
   
    client.publish("esp8266_sw/from", "Switch Off ");
    sw_statu=0;
    client.publish("esp8266_sw/from/status","0");
   
  }
  if (mqtt_reciver.equals(SW_ON))
  {
    client.publish("esp8266_sw/from", "Switch On");
    sw_statu=1;
    client.publish("esp8266_sw/from/status","1");
     
  }
  if (mqtt_reciver.equals(GET_TEMP))
  {
    
  }
  if (mqtt_reciver.equals(GET_HUM))
  {
   
  }
  if (mqtt_reciver.equals(GET_ALT))
  {
    
  }
 if (mqtt_reciver.equals("REQUEST"))
  {
   client.publish("esp8266_sw/from/status","3");
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
      digitalWrite(D4, LOW);
      delay(250);
      digitalWrite(D4, HIGH);
      delay(250);
      digitalWrite(D4, LOW);
      delay(250);
      digitalWrite(D4, HIGH);
      delay(250);
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
      digitalWrite(D4, LOW);
      delay(125);
      digitalWrite(D4, HIGH);
      delay(125);
      digitalWrite(D4, LOW);
      delay(125);
      digitalWrite(D4, HIGH);
      delay(125);
      digitalWrite(D4, LOW);
      delay(125);
      digitalWrite(D4, HIGH);
      delay(125);
      digitalWrite(D4, LOW);
      delay(125);
      digitalWrite(D4, HIGH);
      delay(125);
      digitalWrite(D4, LOW);
      delay(125);
      break;
    }
    digitalWrite(D4, LOW);
    delay(500); // 这个地方一定要加延时，否则极易崩溃重启
    digitalWrite(D4, HIGH);
    delay(500);
  }
}

void Switch_key()
{

  count++;
  if (count > 1)
    count = 0;


  if (count == 0)
  {

    digitalWrite(D4, LOW);
    WiFi.setAutoConnect(true);

  }
  if (count == 1)
  {

    digitalWrite(D4, HIGH);

  }
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
    if (client.connect("ESP8266Client2",mqttUser, mqttPassword)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("esp8266_sw/from/status", "ok");
    
      client.subscribe("esp8266_sw/to");
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
 
  pinMode(D4, OUTPUT);
  pinMode(D1, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(D3), Switch_key , FALLING);
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
    if (client.connect("ESP8266Client2", mqttUser, mqttPassword )) {
      Serial.println("connected");
      client.publish("esp8266_sw/from/status", "ok");
      digitalWrite(D4, LOW);
      delay(200);
      digitalWrite(D4, HIGH);
      delay(250);
      digitalWrite(D4, LOW);
      delay(200);
      digitalWrite(D4, HIGH);
      delay(200);
      digitalWrite(D4, LOW);
      
    } else {
      Serial.println("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("esp8266_sw/to");

}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  digitalWrite(D1, sw_statu);
 
  if (WiFi.status() != WL_CONNECTED)
  {
     
      digitalWrite(D4, LOW);
      delay(120);
      digitalWrite(D4, HIGH);
      delay(120);
      digitalWrite(D4, LOW);
      delay(120);
      digitalWrite(D4, HIGH);
      delay(120);
      digitalWrite(D4, LOW);
      delay(120);
      ESP.restart();
   

  }
  

}
