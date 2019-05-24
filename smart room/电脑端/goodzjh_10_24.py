# -*- coding: utf-8 -*-

"""
Module implementing MainWindow.
"""

from PyQt5.QtCore import pyqtSlot
from PyQt5.QtWidgets import QMainWindow
from PyQt5 import QtCore, QtGui, QtWidgets
from Ui_goodzjh_10_24 import Ui_MainWindow
import time, threading
import paho.mqtt.client as mqtt  
import datetime
import time
from PyQt5.QtWidgets import QApplication, QWidget, QMessageBox
from PyQt5.QtCore import QDate,   QDateTime , QTime
#HOST = "m14.cloudmqtt.com"
#PORT = 13713
HOST = "47.102.159.197"
PORT = 1883

class MainWindow(QMainWindow, Ui_MainWindow):
    """
    Class documentation goes here.
    """
    def __init__(self, parent=None):
        """
        Constructor
        
        @param parent reference to the parent widget
        @type QWidget
        """
        super(MainWindow, self).__init__(parent)
        self.setupUi(self)
    
    @pyqtSlot()
    def on_pushButton_clicked(self):
        """
        Slot documentation goes here.
        """
        client.publish("esp8266/to", "GET_ALL", qos=0, retain=False)
        client.publish("esp8266/to", "REQUEST", qos=0, retain=False)
    @pyqtSlot()
    def on_pushButton_2_clicked(self):
        """
        Slot documentation goes here.
        """
        client.publish("esp8266_sw/to", "SW_ON", qos=0, retain=False)
    
    @pyqtSlot()
    def on_pushButton_3_clicked(self):
        """
        Slot documentation goes here.
        """
        client.publish("esp8266_sw/to", "SW_OFF", qos=0, retain=False)
    @pyqtSlot()
    def on_actionQUIT_triggered(self):
        """
        Slot documentation goes here.
        """
        reply = QMessageBox.question(self, '信息', '确认退出吗？', 
        QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        if reply ==QMessageBox.Yes:
            sys.exit()
        
    
    @pyqtSlot()
    def on_actionAbout_triggered(self):
        """
        Slot documentation goes here.
        """
        QMessageBox.about(self,"About","张家辉的毕业设计.\n 201540438126.")
        
    
    @pyqtSlot()
    def on_dang0_clicked(self):
        """
        Slot documentation goes here.
        """
        client.publish("esp8266_fan/to", "REQUEST", qos=0, retain=False)
        client.publish("esp8266_fan/to", "0", qos=0, retain=False)
    
    @pyqtSlot()
    def on_dang3_clicked(self):
        """
        Slot documentation goes here.
        """
        client.publish("esp8266_fan/to", "3", qos=0, retain=False)
        
    
    @pyqtSlot()
    def on_dang1_clicked(self):
        """
        Slot documentation goes here.
        """
        client.publish("esp8266_fan/to", "1", qos=0, retain=False)
       
    
    @pyqtSlot()
    def on_dang2_clicked(self):
        """
        Slot documentation goes here.
        """
        client.publish("esp8266_fan/to", "2", qos=0, retain=False)
    
    @pyqtSlot()
    def on_pushButton_4_clicked(self):    #火焰
        """
        Slot documentation goes here.
        """
        if ui.pushButton_4.text()=="关":
            client.publish("esp8266_rhq/to", "OPEN_H", qos=0, retain=False)
            ui.pushButton_4.setText("开")
        else:
            client.publish("esp8266_rhq/to", "CLOSE_H", qos=0, retain=False)
            ui.pushButton_4.setText("关")
    
    @pyqtSlot()
    def on_pushButton_5_clicked(self):    #人体
        """
        Slot documentation goes here.
        """
        if ui.pushButton_5.text()=="关":
            client.publish("esp8266_rhq/to", "OPEN_R", qos=0, retain=False)
            ui.pushButton_5.setText("开")
        else:
            client.publish("esp8266_rhq/to", "CLOSE_R", qos=0, retain=False)
            ui.pushButton_5.setText("关")
    @pyqtSlot(str)
    def on_comboBox_activated(self, p0):
        print(ui.comboBox.currentText())
        if ui.comboBox.currentText()=="风扇":
            ui.comboBox_2.clear()
            ui.comboBox_2.addItem('请选择')
            ui.comboBox_2.addItem('零档')
            ui.comboBox_2.addItem('一档')
            ui.comboBox_2.addItem('二档')
            ui.comboBox_2.addItem('三档')
        if ui.comboBox.currentText()=="插座":
            ui.comboBox_2.clear()
            ui.comboBox_2.addItem('请选择')
            ui.comboBox_2.addItem('开')
            ui.comboBox_2.addItem('关')
        if ui.comboBox.currentText()=="火焰报警":
            ui.comboBox_2.clear()
            ui.comboBox_2.addItem('请选择')
            ui.comboBox_2.addItem('开')
            ui.comboBox_2.addItem('关')
        if ui.comboBox.currentText()=="人体报警":
            ui.comboBox_2.clear()
            ui.comboBox_2.addItem('请选择')
            ui.comboBox_2.addItem('开')
            ui.comboBox_2.addItem('关')
        if ui.comboBox.currentText()=="请选择":
            ui.comboBox_2.clear()
        
        
    
    
    @pyqtSlot()
    def on_pushButton_6_clicked(self):
        nowtime=list(time.localtime( time.time() ))
        Datetime=str(ui.dateTimeEdit.date())[19:-1].split(',')+str(ui.dateTimeEdit.time())[19:-1].split(',')
        Datetime1=list(map(int, Datetime)) 
        nowtime1=nowtime[0:5]
        for  i in  range(len(nowtime1)):
            
            if nowtime1[i]<Datetime1[i]: 
                if(ui.comboBox.currentText()=="请选择"):
                    QMessageBox.warning(self, "温馨提醒","请选择操作设备",QMessageBox.Cancel)
                    break
                elif(ui.comboBox_2.currentText()=="请选择"):
                    QMessageBox.warning(self, "温馨提醒","请选择具体操作",QMessageBox.Cancel)
                    break
                else:
                    do=str(ui.comboBox.currentIndex())+str(ui.comboBox_2.currentIndex())
                    t1=(Datetime1)
                    r=str(t1[0]).zfill(4)+str(t1[1]).zfill(2)+str(t1[2]).zfill(2)+str(t1[3]).zfill(2)+str(t1[4]).zfill(2)
                    
                    print(r+do)
                    client.publish("TimeTask_Request", r+do, qos=1, retain=False)
                    break
                
            elif nowtime1[i]>Datetime1[i]:
                QMessageBox.warning(self, "温馨提醒","设置的时间请晚于现在",QMessageBox.Cancel)
                break
               
        
        
        
        
if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    ui = MainWindow()
        
        # 当连接上服务器后回调此函数  
    def on_connect(client, userdata, flags, rc):  
        if rc == 0:
            ui.label_9.setText("连接成功")
            client.publish("esp8266/to", "REQUEST", qos=0, retain=False)
            client.publish("esp8266_sw/to", "REQUEST", qos=0, retain=False)
            client.publish("esp8266_fan/to", "REQUEST", qos=0, retain=False)
            client.publish("esp8266_rhq/to", "REQUEST", qos=0, retain=False)
        else:
            ui.label_9.setText("连接失败")
        
      
        # 放在on_connect函数里意味着  
        # 重新连接时订阅主题将会被更新  
        client.subscribe("esp8266_sw/from/status")       #开关状态
        client.subscribe("esp8266/from/status")            #温湿度设备状态
        client.subscribe("esp8266/from/PUB_ALL")           #获取温湿度
        client.subscribe("esp8266_fan/from/status")       
        client.subscribe("esp8266_rhq/from/status")
        client.subscribe("esp8266_rhq/from/HBJ")
        client.subscribe("esp8266_rhq/from/RBJ")
        client.subscribe("esp8266_rhq/from/R")
        client.subscribe("esp8266_rhq/from/H")
        client.subscribe("esp8266_rhq/from/q")
        client.subscribe("TimeTask_Reply")
    # 从服务器接受到消息后回调此函数  
    def on_message(client, userdata, msg): 
        if  msg.topic=="esp8266/from/PUB_ALL":
            str1=str(bytes.decode(msg.payload)).split(';')
            print(str1)
            ui.label_5.setText(str1[0])
            ui.label_6.setText(str1[2])
            ui.label_7.setText(str1[1])
            ui.label_15.setText(str1[3])
            ui.label_13.setText(time.strftime('%Y-%m-%d %H:%M:%S',time.localtime(time.time())))
        if  msg.topic=="esp8266/from/status":
            ui.label_11.setText("在线")
            
        if  msg.topic=="esp8266_sw/from/status":
            ui.label_20.setText("在线")
            if bytes.decode(msg.payload)=="1":
                ui.label_18.setText("开")
            else:
                ui.label_18.setText("关")
        if msg.topic=="esp8266_fan/from/status":
            ui.fansbzt.setText("在线")
            if bytes.decode(msg.payload)=="0":
                ui.fanfszt.setText("关")
            if bytes.decode(msg.payload)=="1":
                ui.fanfszt.setText("一档")
            if bytes.decode(msg.payload)=="2":
                ui.fanfszt.setText("二档")
            if bytes.decode(msg.payload)=="3":
                ui.fanfszt.setText("三档")
        if msg.topic=="esp8266_rhq/from/status":
            ui.label_28.setText("在线")
            client.publish("esp8266_rhq/to", "CLOSE_R", qos=0, retain=False)
            client.publish("esp8266_rhq/to", "CLOSE_H", qos=0, retain=False)
        if msg.topic=="esp8266_rhq/from/q":
            ui.label_29.setText(bytes.decode(msg.payload));
        if msg.topic=="esp8266_rhq/from/HBJ":
            ui.label_30.setText("检测到火焰，请及时处理")
            time.sleep(1)
            ui.label_30.setText("")
        if msg.topic=="esp8266_rhq/from/RBJ":
            ui.label_30.setText("有人闯入，请及时处理")
            time.sleep(1)
            ui.label_30.setText("")
        if msg.topic=="TimeTask_Reply":
            ui.pushButton_6.setText("发送成功")
            time.sleep(1)
            ui.pushButton_6.setText("发送命令")
        print("主题:"+msg.topic+" 消息:"+bytes.decode(msg.payload)) 
    
       
    client = mqtt.Client()  
    #参数有 Client(client_id="", clean_session=True, userdata=None, protocol=MQTTv311, transport="tcp")  
    client.on_connect = on_connect #设置连接上服务器回调函数  
    client.on_message = on_message  #设置接收到服务器消息回调函数
#    client.username_pw_set("dmrszgvw",password="iVfd7J2l4ZZ0")
    client.username_pw_set("admin",password="public")
    client.connect(HOST,PORT, 60)  #连接服务器,端口为1883,维持心跳为60秒  
    client.loop_start()
    ui.show()
    sys.exit(app.exec_())
    
    
