package com.example.smartroom;

import android.provider.ContactsContract;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.CompoundButton;
import android.widget.ImageView;
import android.widget.Switch;
import android.widget.Toast;

public class WaringActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_waring);
        Switch fire=(Switch) findViewById(R.id.fire_sw);
        Switch man = (Switch) findViewById(R.id.man_sw);
        final ImageView image_fire = (ImageView)findViewById(R.id.waring_fire);
        final ImageView image_man = (ImageView)findViewById(R.id.waring_man);
        fire.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked)
                {
                    image_fire.setImageResource(R.drawable.fire1);
                    MyMqttService.publish("OPEN_H", 0, "esp8266_rhq/to");
                    Toast.makeText(getApplicationContext(),"火焰报警开启",Toast.LENGTH_SHORT).show();
                }else {
                    image_fire.setImageResource(R.drawable.fire0);
                    MyMqttService.publish("CLOSE_H", 0, "esp8266_rhq/to");
                    Toast.makeText(getApplicationContext(),"火焰报警关闭",Toast.LENGTH_SHORT).show();
                }
            }
        });
        man.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
                if(isChecked){
                    image_man.setImageResource(R.drawable.man1);
                    MyMqttService.publish("OPEN_R", 0, "esp8266_rhq/to");
                    Toast.makeText(getApplicationContext(),"人体报警开启",Toast.LENGTH_SHORT).show();
                }else{
                    image_man.setImageResource(R.drawable.man0);
                    MyMqttService.publish("CLOSE_R", 0, "esp8266_rhq/to");
                    Toast.makeText(getApplicationContext(),"火焰报警关闭",Toast.LENGTH_SHORT).show();
                }
            }
        });
    }
}
