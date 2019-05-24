package com.example.smartroom;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.icu.text.UnicodeSetSpanner;
import android.media.Image;
import android.media.MediaPlayer;
import android.media.RingtoneManager;
import android.os.Vibrator;
import android.support.v4.app.NotificationCompat;
import android.support.v4.app.NotificationManagerCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.AppCompatImageView;
import android.view.View;
import android.widget.Button;
import android.widget.ImageButton;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {
    public static final String ACTION_UPDATEUI = "action.updateUI";
    private Vibrator vibrator;//震动
    private MediaPlayer mediaplayer;//提示音
    MyReceiver broadcastReceiver;


        public  void waring(int i,int m)
         {
            vibrator = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
            vibrator.vibrate(m);
        // 铃声（这里是先释放掉内存）
             if(i==1) {
                 if (mediaplayer != null) {
                     mediaplayer.stop();
                     mediaplayer.release();
                     mediaplayer = null;
                 }
                 mediaplayer = new MediaPlayer();
                 try {
                     mediaplayer.setDataSource(getApplicationContext(), RingtoneManager.getDefaultUri(RingtoneManager.TYPE_NOTIFICATION));
                     mediaplayer.prepare();
                 } catch (Exception e) {
                     e.printStackTrace();
                 }
                 mediaplayer.start();
             }
    }
        @Override
        protected void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            getSupportActionBar().hide();
            setContentView(R.layout.activity_main);
            ImageButton bt_key = (ImageButton)findViewById(R.id.bt_key);
            ImageButton bt_fan = (ImageButton)findViewById(R.id.bt_fan);
            ImageButton bt_wsd = (ImageButton)findViewById(R.id.bt_wsd);
            ImageButton bt_timetask = (ImageButton)findViewById(R.id.bt_timetask);
            ImageButton bt_waring = (ImageButton)findViewById(R.id.bt_waring);
            ImageButton bt_setting = (ImageButton)findViewById(R.id.setting);
            bt_key.setEnabled(false);
            bt_fan.setEnabled(false);
            bt_wsd.setEnabled(false);
            bt_waring.setEnabled(false);
            IntentFilter filter = new IntentFilter();
            filter.addAction(ACTION_UPDATEUI);
            broadcastReceiver = new MyReceiver();
            registerReceiver(broadcastReceiver, filter);
            MyMqttService.startService(this); //开启服务

            bt_setting.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {


                waring(0,100);
                Intent intent = new Intent(MainActivity.this,EsptouchDemoActivity.class);
                startActivity(intent);

            }
        });
        bt_waring.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                waring(0,100);
                Intent intent = new Intent(MainActivity.this,WaringActivity.class);
                startActivity(intent);
            }
        });
        bt_timetask.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                waring(0,100);
                Intent intent = new Intent(MainActivity.this,TimetaskActivity.class);
                startActivity(intent);
            }
        });
        bt_wsd.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                waring(0,100);
                Intent intent = new Intent(MainActivity.this,THPActivity.class);
                startActivity(intent);
            }
        });
        bt_fan.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                waring(0,100);
                Intent intent = new Intent(MainActivity.this,FanActivity.class);
                startActivity(intent);
            }
        });
        bt_key.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                waring(0,100);
                Intent intent = new Intent(MainActivity.this,KeyActivity.class);
                startActivity(intent);
            }
        });
    }

    private class MyReceiver extends BroadcastReceiver {

        ImageButton bt_key = (ImageButton)findViewById(R.id.bt_key);
        ImageButton bt_fan = (ImageButton)findViewById(R.id.bt_fan);
        ImageButton bt_wsd = (ImageButton)findViewById(R.id.bt_wsd);
        ImageButton bt_waring = (ImageButton)findViewById(R.id.bt_waring);
        @Override
        public void onReceive(Context context, Intent intent) {
           if(intent.getExtras().getString("esp8266_sw/from/status")!=null)
           {
               bt_key.setImageResource(R.drawable.czkai);
               bt_key.setEnabled(true);
           }
            if(intent.getExtras().getString("esp8266_fan/from/status")!=null)
            {
                bt_fan.setImageResource(R.drawable.fankai);
                bt_fan.setEnabled(true);

            }
            if(intent.getExtras().getString("esp8266_rhq/from/status")!=null)
            {
                bt_waring.setImageResource(R.drawable.bjkai);
                bt_waring.setEnabled(true);
            }
            if(intent.getExtras().getString("esp8266/from/status")!=null)
            {
                bt_wsd.setImageResource(R.drawable.wsdkai);
                bt_wsd.setEnabled(true);
            }
            if(intent.getExtras().getString("esp8266_rhq/from/HBJ")!=null)
            {

                Toast.makeText(getApplicationContext(),"检测到火焰请及时处理", Toast.LENGTH_SHORT).show();
                waring(1,5000);
            }
            if(intent.getExtras().getString("esp8266_rhq/from/RBJ")!=null)
            {

                Toast.makeText(getApplicationContext(),"有人闯入请及时处理", Toast.LENGTH_SHORT).show();
                waring(1,5000);

            }
            if(intent.getExtras().getString("TimeTask_Reply")!=null)
            {

                Toast.makeText(getApplicationContext(),"定时命令发送成功", Toast.LENGTH_SHORT).show();
                waring(1,50);

            }
        }

    }
}
