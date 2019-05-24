#!/usr/bin/python3
# -*- coding: utf-8 -*-

import pymysql
import paho.mqtt.client as mqtt
import datetime,time
import threading
HOST = "47.102.159.197"
PORT = 1883




if __name__ == "__main__":
    
    # 打开数据库连接
    db = pymysql.connect("rm-bp17z4nh0yd2o0ff6jo.mysql.rds.aliyuncs.com","root","Zjh19961020","bysj" )
    # 使用 cursor() 方法创建一个游标对象 cursor
    cursor = db.cursor()
    cursor1 =db.cursor()
    # 使用 execute() 方法执行 SQL，如果表不存在则创建
    sql = """ CREATE TABLE IF NOT EXISTS THP(
              TIME TIMESTAMP,TEM CHAR(8),HUM CHAR(8),PRE CHAR(8))"""

    sql1= """ CREATE TABLE IF NOT EXISTS MQ2(
              TIME TIMESTAMP,GAS CHAR(8))"""
    cursor.execute(sql)
    cursor1.execute(sql1)
    
    def on_connect(client, userdata, flags, rc):  
        if rc == 0:
            print("connect mqtt success")
        else:
            print("connect mqtt fail")
            
        
      
        # 放在on_connect函数里意味着  
        # 重新连接时订阅主题将会被更新
        client.subscribe("esp8266_rhq/from/q")
        client.subscribe("esp8266_sw/from/status")  
        client.subscribe("esp8266/from/PUB_ALL")
        client.subscribe("mysql")
        
    # 从服务器接受到消息后回调此函数  
    def on_message(client, userdata, msg): 
        if  msg.topic=="esp8266/from/PUB_ALL":
            str1=str(bytes.decode(msg.payload)).split(';')
            print(str1)
            cursor.execute('insert into thp values(%s,%s,%s,%s)'%\
                           (time.strftime("%Y%m%d%H%M%S"),str1[0],str1[2],str1[1]))
            db.commit()
        if  msg.topic=="mysql":
            if str(bytes.decode(msg.payload))=="clean thp":
                cursor.execute("truncate table thp")
                db.commit()
            if str(bytes.decode(msg.payload))=="clean mq2":
                cursor1.execute("truncate table MQ2")
                db.commit()
        if  msg.topic=="esp8266_rhq/from/q":
            str2=str(bytes.decode(msg.payload))
            print(str2)
            cursor1.execute('insert into MQ2 values(%s,%s)'%\
                           (time.strftime("%Y%m%d%H%M%S"),str2))
            db.commit()

        print("主题:"+msg.topic+" 消息:"+bytes.decode(msg.payload))

    def time_5min():
        client.publish("esp8266/to", "GET_ALL", qos=0, retain=False)
        client.publish("esp8266_rhq/to", "GET_Q", qos=0, retain=False)
        timer=threading.Timer(60,time_5min)
        timer.start()
    client = mqtt.Client()  #创建一个Client对象
    client.on_connect = on_connect #设置连接上服务器回调函数  
    client.on_message = on_message  #设置接收到服务器消息回调函数
    client.username_pw_set("admin",password="public")
    client.connect(HOST,PORT, 60)  #连接服务器,端口为1883,维持心跳为60秒  
    client.loop_start()
    time_5min()
