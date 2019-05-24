
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
char pre_data[7];
char vol_data[6];
String mqtt_reciver = "";
char* LED_STATUS = "";
String LED_ON = "LED_ON";
String LED_OFF = "LED_OFF";
String GET_TEMP = "GET_TEMP";
String GET_HUM = "GET_HUM";
String GET_PRE = "GET_PRE";
String GET_VOL = "GET_VOL";
String RE = "REQUEST";
String GET_ALL = "GET_ALL";
char All[32];
static int count = 0;

void oled_display_string(char*a);
int Count_Num(double a);   
void reconnect();
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
    digitalWrite(D4, LOW);
    oled_display_string("\n\n\n LED ON\r");
    client.publish("esp8266/from/led", "LED ON ");
  }
  if (mqtt_reciver.equals(LED_OFF))
  {
    digitalWrite(D4, HIGH);
    oled_display_string("\n\n\n LED_OFF\r");
    client.publish("esp8266/from/led", "LED OFF");
  }
  if (mqtt_reciver.equals(GET_TEMP))
  {
    char t[16] = "\n\n tempe:\n\n    ";
    dtostrf(sht30.cTemp, Count_Num(sht30.cTemp), 4-Count_Num(sht30.cTemp), temp_data);
    client.publish("esp8266/from/temperature", temp_data);
    temp_data[5] = '\r';
    Serial.println(strcat(t, temp_data));
    oled_display_string(strcat(t, temp_data));
  }
  if (mqtt_reciver.equals(GET_HUM))
  {
    char t[16] = "\n\n humid:\n\n    ";
    dtostrf(sht30.humidity,Count_Num(sht30.humidity), 4-Count_Num(sht30.humidity), hum_data);
    client.publish("esp8266/from/Humidity", hum_data);
    hum_data[5] = '\r';
    Serial.println(strcat(t, hum_data));
    oled_display_string(strcat(t, hum_data));
  }
  if (mqtt_reciver.equals(GET_PRE))
  {
    char t[16] = "\n\n Press:\n\n    ";
    dtostrf(bmp.readPressure()/1000,Count_Num(bmp.readPressure()/1000), 4-Count_Num(bmp.readPressure()/1000), pre_data);
    client.publish("esp8266/from/Pressure", pre_data);
    pre_data[5] = '\r';
    Serial.println(strcat(t, pre_data));
    oled_display_string(strcat(t, pre_data));
  }
 if (mqtt_reciver.equals(GET_VOL))
  {
    char t[16] = "\n\n Volta:\n\n    ";
    dtostrf((float)ESP.getVcc()/1000,Count_Num((float)ESP.getVcc()/1000), 4-Count_Num((float)ESP.getVcc()/1000), vol_data);
    client.publish("esp8266/from/Voltage", vol_data);
    vol_data[5] = '\r';
    Serial.println(strcat(t, vol_data));
    oled_display_string(strcat(t, vol_data));
  }
  if (mqtt_reciver.equals(RE))
  {
    client.publish("esp8266/from/status","ok" );
  }
   if (mqtt_reciver.equals(GET_ALL))
  {
    
    sprintf(All,"%.2f;%d;%.2f;%.2f",sht30.cTemp,bmp.readPressure(),sht30.humidity,(float)ESP.getVcc()/1000);
    Serial.println(bmp.readPressure()/1000);
    client.publish("esp8266/from/PUB_ALL",All);
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
  oled_display_string("\n\n Smart \n Config...\r");
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
    client.publish("esp8266_sw/to","SW_ON");

  }
  if (count == 1)
  {

    digitalWrite(D4, HIGH);
    WiFi.setAutoConnect(false);
    client.publish("esp8266_sw/to","SW_OFF");

  }


}
void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client1",mqttUser, mqttPassword)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("esp8266/from/status", "ok");
      // ... and resubscribe
      oled_display_string("\n\n\n Welcome\r");
      client.subscribe("esp8266/to");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      oled_display_string("\n\n\n failed\r");
      // Wait 5 seconds before retrying
      delay(5000);
      if (WiFi.status() != WL_CONNECTED)
      {
        break;
        }
    }
}
}

void oled_display_string(char *a)
{
  oled.begin();
  oled.clear(ALL);
  oled.clear(PAGE);

  while (*a != '\r')
  {
    oled.write(*a);
    Serial.print(*a);
    a += 1;
  }
  oled.display();
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

void event()
{
  sprintf(All,"%.2f;%d;%.2f;%.2f",sht30.cTemp,bmp.readPressure(),sht30.humidity,(float)ESP.getVcc()/1000); 
  client.publish("esp8266/from/PUB_ALL",All);
}



void setup()
{
  Serial.begin(115200);
  oled.begin();
  bmp.begin();
  oled.clear(ALL);
  oled.clear(PAGE);
  oled.display();
  oled_display_string("\n\n\n Loading..\r");
  pinMode(D4, OUTPUT);

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
    if (client.connect("ESP8266Client1", mqttUser, mqttPassword )) {
      Serial.println("connected");
      digitalWrite(D4, LOW);
      delay(200);
      digitalWrite(D4, HIGH);
      delay(250);
      digitalWrite(D4, LOW);
      delay(200);
      digitalWrite(D4, HIGH);
      oled_display_string("\n\n\n Welcome\r");
      client.publish("esp8266/from/status1","success" );
      

    } else {
      Serial.println("failed with state ");
      Serial.print(client.state());
      oled_display_string("\n\n\n failed\r");
      delay(2000);
    }
  }
  client.subscribe("esp8266/to");

}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  
  sht30.get();
  if (WiFi.status() != WL_CONNECTED)
 {
      digitalWrite(D4, LOW);
      delay(120);
      digitalWrite(D4, HIGH);
      delay(120);
    oled_display_string("\n\n Lose \n Connect\r");
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
     
    oled_display_string("\n\n Please \n Restart\r");
    
      delay(120);
      digitalWrite(D4, LOW);
      delay(120);
      digitalWrite(D4, HIGH);
      delay(120);
      digitalWrite(D4, LOW);
      delay(120);
      digitalWrite(D4, HIGH);
      delay(120);
      
  }
  if(ESP.getVcc()<2000)
  {
    oled_display_string("\n\n POWER \n LOW\r");
    
    }

}
