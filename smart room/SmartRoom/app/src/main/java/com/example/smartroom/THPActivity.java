package com.example.smartroom;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

public class THPActivity extends AppCompatActivity {
    MyReceiver1 broadcastReceiver;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_thp);
        IntentFilter filter = new IntentFilter();
        filter.addAction("GET_THP");
        broadcastReceiver = new MyReceiver1();
        registerReceiver(broadcastReceiver, filter);
        Button button = (Button)findViewById(R.id.b1);
        button.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                MyMqttService.publish("GET_ALL", 0, "esp8266/to");
            }
        });
    }
    private class MyReceiver1 extends BroadcastReceiver{
        @Override
        public void onReceive(Context context, Intent intent) {
            TextView t = (TextView)findViewById(R.id.TVT);
            TextView p= (TextView)findViewById(R.id.TVP);
            TextView h= (TextView)findViewById(R.id.TVH);
            //Toast.makeText(getApplicationContext(),intent.getExtras().getString("esp8266/from/PUB_ALL"),Toast.LENGTH_SHORT).show();
            String data = intent.getExtras().getString("esp8266/from/PUB_ALL");
            String []data1=data.split("[;]");
            t.setText(data1[0]+"℃");
            p.setText(data1[1]+"Pa");
            h.setText(data1[2]+"%");
        }
    }
}
