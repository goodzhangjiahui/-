package com.example.smartroom;

import android.content.Context;
import android.content.Intent;
import android.graphics.drawable.AnimationDrawable;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;


public class FanActivity extends AppCompatActivity {
    int i=0;
     SeekBar seekBar;
     TextView textView;
     ImageView imageView;
    AnimationDrawable animationDrawable;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_fan);

//        final GifImageView gifImageView =(GifImageView) findViewById(R.id.gifview);
//        final GifDrawable gifDrawable=(GifDrawable)gifImageView.getDrawable();
//        final android.widget.MediaController mediaController = new android.widget.MediaController(this);
//        mediaController.setMediaPlayer((GifDrawable) gifImageView.getDrawable());
//        gifDrawable.setLoopCount(100);
         seekBar = (SeekBar) findViewById(R.id.seekBar);
         textView = (TextView)findViewById(R.id.textView);
         imageView = (ImageView) findViewById(R.id.imageView3);
         imageView.setImageResource(R.drawable.layout);
         animationDrawable = (AnimationDrawable) imageView.getDrawable();
         seekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                   String a = Integer.toString(progress);
                   if(a.equals("0"))
                   {
                       textView.setText("关");
                        animationDrawable.stop();
                       MyMqttService.publish("0", 0, "esp8266_fan/to");
                   }
                   if(a.equals("1"))
                   {
                          textView.setText("一档");
                          imageView.setImageResource(R.drawable.layout);
                          animationDrawable = (AnimationDrawable) imageView.getDrawable();
                          animationDrawable.start();
                          MyMqttService.publish("1", 0, "esp8266_fan/to");

                   }
                   if(a.equals("2"))
                   {
                         textView.setText("二档");
                         imageView.setImageResource(R.drawable.layout2);
                         animationDrawable = (AnimationDrawable) imageView.getDrawable();
                         animationDrawable.start();
                         MyMqttService.publish("2", 0, "esp8266_fan/to");

                   }
                   if(a.equals("3")) {
                       textView.setText("三档");
                       imageView.setImageResource(R.drawable.layout3);
                       animationDrawable = (AnimationDrawable) imageView.getDrawable();
                       animationDrawable.start();
                       MyMqttService.publish("3", 0, "esp8266_fan/to");

                   }

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });


        //mediaController.show();


    }

}
