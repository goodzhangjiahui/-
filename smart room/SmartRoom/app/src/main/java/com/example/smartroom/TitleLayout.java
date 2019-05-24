package com.example.smartroom;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.support.annotation.Nullable;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.TouchDelegate;
import android.view.View;
import android.widget.ImageButton;
import android.widget.LinearLayout;
public class TitleLayout extends LinearLayout {
    public TitleLayout(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        LayoutInflater.from(context).inflate(R.layout.title,this);
        ImageButton back = (ImageButton) findViewById(R.id.back);
        ImageButton about = (ImageButton) findViewById(R.id.about);
        about.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                AlertDialog.Builder dialog = new AlertDialog.Builder(getContext());
                dialog.setTitle("关于");
                dialog.setMessage("2015级毕业设计--智能宿舍" +
                        "\n\n\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tby gooodzjh");
                dialog.setCancelable(false);
                dialog.setPositiveButton("I know", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface dialog, int which) {

                    }
                });
                dialog.show();
            }
        });

        back.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                ((Activity)getContext()).finish();
            }
        });

    }
}

