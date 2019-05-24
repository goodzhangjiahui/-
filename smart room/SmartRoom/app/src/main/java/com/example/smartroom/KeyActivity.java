package com.example.smartroom;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.Switch;

public class KeyActivity extends AppCompatActivity {
    ImageView imageView;
    Switch aSwitch;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_key);
        imageView = (ImageView) findViewById(R.id.imagev);
        aSwitch = (Switch) findViewById(R.id.switch1);
        aSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if (isChecked) {
                    imageView.setImageResource(R.drawable.czkai);
                    MyMqttService.publish("SW_ON", 0, "esp8266_sw/to");
                } else {
                    imageView.setImageResource(R.drawable.czguan);
                    MyMqttService.publish("SW_OFF", 0, "esp8266_sw/to");
                }
            }
        });
    }
}
