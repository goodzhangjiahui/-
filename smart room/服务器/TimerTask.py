#!/usr/bin/python3
# -*- coding: utf-8 -*-


import paho.mqtt.client as mqtt
import datetime,time
import threading
HOST = "47.102.159.197"
PORT = 1883
TaskList=[];
TaskList1=[];
lock=threading.Lock()

if __name__ == "__main__":

    def do_job(str1):
        print(str1)
        if(str1[12:14]=="11"): 
            client.publish("esp8266_fan/to", "0", qos=0, retain=False)
        if(str1[12:14]=="12"):
            client.publish("esp8266_fan/to", "1", qos=0, retain=False)
        if(str1[12:14]=="13"):
            client.publish("esp8266_fan/to", "2", qos=0, retain=False)
        if(str1[12:14]=="14"):
            client.publish("esp8266_fan/to", "3", qos=0, retain=False)
        if(str1[12:14]=="21"):
            client.publish("esp8266_sw/to", "SW_ON", qos=0, retain=False)
        if(str1[12:14]=="22"):
            client.publish("esp8266_sw/to", "SW_OFF", qos=0, retain=False)
        if(str1[12:14]=="31"):
            client.publish("esp8266_rhq/to", "OPEN_H", qos=0, retain=False)
        if(str1[12:14]=="32"):
            client.publish("esp8266_rhq/to", "CLOSE_H", qos=0, retain=False)
        if(str1[12:14]=="41"):
            client.publish("esp8266_rhq/to", "OPEN_R", qos=0, retain=False)
        if(str1[12:14]=="42"):
            client.publish("esp8266_rhq/to", "CLOSE_R", qos=0, retain=False)

    def execute_task():
        
        t1=list(time.localtime( time.time() ))
        t2=str(t1[0]).zfill(4)+str(t1[1]).zfill(2)+str(t1[2]).zfill(2)+str(t1[3]).zfill(2)+str(t1[4]).zfill(2)
        lock.acquire()
        print(TaskList)
        index=[i for i,m in enumerate(TaskList) if m.find(t2)!=-1 ]
        print(index)
        for i in range(len(index)):
            do_job(TaskList[0])
            TaskList.pop(0)
        lock.release()

        timer=threading.Timer(20,execute_task)
        timer.start()
    def on_connect(client, userdata, flags, rc):  
        if rc == 0:
            print("connect mqtt success")
        else:
            print("connect mqtt fail")
            

        client.subscribe("TimeTask_Request")

    def on_message(client, userdata, msg): 
        print("topic:"+msg.topic+" message:"+bytes.decode(msg.payload))
        if  msg.topic=="TimeTask_Request":
            client.publish("TimeTask_Reply", "1", qos=0, retain=False)
            temp=str(bytes.decode(msg.payload))
            lock.acquire()
            if(TaskList.count(temp)==0):
                     TaskList.append(temp)  
            TaskList.sort()
            lock.release()
            print(TaskList)
        
    
    client = mqtt.Client()  #创建一个Client对象
    client.on_connect = on_connect #设置连接上服务器回调函数  
    client.on_message = on_message  #设置接收到服务器消息回调函数
    client.username_pw_set("admin",password="public")
    client.connect(HOST,PORT, 10)  #连接服务器,端口为1883,维持心跳为60秒  
    client.loop_start()
    execute_task()
