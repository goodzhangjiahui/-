

#include <ESP8266WiFi.h>
#include<PubSubClient.h>
#include<String.h>
#include <EEPROM.h>
ADC_MODE(ADC_TOUT);
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
char Q_data[6];
String mqtt_reciver = "";
String GET_Q = "GET_Q";
String OPEN_H = "OPEN_H";
String OPEN_R = "OPEN_R";
String CLOSE_H = "CLOSE_H";
String CLOSE_R = "CLOSE_R";
String RE = "REQUEST";

void reconnect();
void f_event();
void r_event();
WiFiClient espClient;
PubSubClient client(espClient);

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
  
// if (mqtt_reciver.equals(GET_VOL))
//  {
//    char t[16] = "\n\n Volta:\n\n    ";
//    dtostrf((float)ESP.getVcc()/1000,Count_Num((float)ESP.getVcc()/1000), 4-Count_Num((float)ESP.getVcc()/1000), vol_data);
//    client.publish("esp8266/from/Voltage", vol_data);
//    vol_data[5] = '\r';
//    Serial.println(strcat(t, vol_data));
//  }
  if (mqtt_reciver.equals(RE))
  {
    client.publish("esp8266_rhq/from/status","ok" );
  } 
  if (mqtt_reciver.equals(GET_Q))
  {
    
    sprintf(Q_data,"%d",analogRead(A0)*6);
    client.publish("esp8266_rhq/from/q",Q_data);
  }
  if (mqtt_reciver.equals(OPEN_H))
  {
    attachInterrupt(digitalPinToInterrupt(D2), f_event , FALLING);
    client.publish("esp8266_rhq/from/H","1");
  }
  if (mqtt_reciver.equals(OPEN_R))
  {
    attachInterrupt(digitalPinToInterrupt(D1), r_event , RISING);
    client.publish("esp8266_rhq/from/R","1");
  }
  if (mqtt_reciver.equals(CLOSE_H))
  {
    detachInterrupt(D2);
    client.publish("esp8266_rhq/from/H","0");
  }
  if (mqtt_reciver.equals(CLOSE_R))
  {
    detachInterrupt(D1);
    client.publish("esp8266_rhq/from/R","0");
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
      break;
    }
    digitalWrite(D4, LOW);
    delay(500); // 这个地方一定要加延时，否则极易崩溃重启
    digitalWrite(D4, HIGH);
    delay(500);
  }
}


void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client_rhq",mqttUser, mqttPassword)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("esp8266_rhq/from/status", "ok");
      // ... and resubscribe
      
      client.subscribe("esp8266_rhq/to");
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
void f_event()
{
    Serial.println("HF");
    client.publish("esp8266_rhq/from/HBJ","1");
}
void r_event()
{
    Serial.println("HR");
    client.publish("esp8266_rhq/from/RBJ","1");
}
//void event()
//{
//  sprintf(All,"%.2f;%d;%.2f;%.2f",sht30.cTemp,bmp.readPressure(),sht30.humidity,(float)ESP.getVcc()/1000); 
//  client.publish("esp8266/from/PUB_ALL",All);
//}



void setup()
{
  Serial.begin(115200);
  
  pinMode(D4, OUTPUT);
  pinMode(D2, INPUT);
  pinMode(D1, INPUT);

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
    if (client.connect("ESP8266Client_rhq", mqttUser, mqttPassword )) {
      Serial.println("connected");
      digitalWrite(D4, LOW);
      delay(200);
      digitalWrite(D4, HIGH);
      delay(250);
      digitalWrite(D4, LOW);
      delay(200);
      digitalWrite(D4, HIGH);
      client.publish("esp8266_rhq/from/status","success" );
      

    } else {
      Serial.println("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
  client.subscribe("esp8266_rhq/to");

}

void loop() {
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
 
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
      digitalWrite(D4, HIGH);
      delay(120);
      digitalWrite(D4, LOW);
      delay(120);
      
  }
  Serial.println(analogRead(A0)*6);
  delay(1000);


}
