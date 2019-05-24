package com.example.smartroom;

import android.app.AlertDialog;
import android.app.DatePickerDialog;
import android.app.LauncherActivity;
import android.app.TimePickerDialog;
import android.content.DialogInterface;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.DatePicker;
import android.widget.EditText;
import android.widget.ImageButton;
import android.widget.LinearLayout;
import android.widget.Spinner;
import android.widget.Switch;
import android.widget.TextView;
import android.widget.TimePicker;
import android.widget.Toast;

import java.util.ArrayList;
import java.util.Calendar;
import java.util.List;

public class TimetaskActivity extends AppCompatActivity {
    private TextView Year;
    private TextView Month;
    private TextView Day;
    private TextView Hour;
    private TextView Min;
    private LinearLayout L1;
    private LinearLayout L2;
    private Spinner shebei;
    private Spinner caozuo;
    private String doing;
    private ImageButton bt_fs;
    private String getdata()
    {
        String Y = Year.getText().toString();
        String M = Month.getText().toString();
        String D = Day.getText().toString();
        String H = Hour.getText().toString();
        String Mi = Min.getText().toString();
        if(M.length()!=2)
        {
            M="0"+M;
        }
        if(D.length()!=2)
        {
            D="0"+D;
        }
        if(H.length()!=2)
        {
            H="0"+H;
        }
        if(Mi.length()!=2)
        {
            Mi="0"+Mi;
        }
        return (Y+M+D+H+Mi);

    }
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        getSupportActionBar().hide();
        setContentView(R.layout.activity_timetask);
        Year=findViewById(R.id.year);
        Month=findViewById(R.id.month);
        Day = findViewById(R.id.day);
        Hour=findViewById(R.id.hour);
        Min=findViewById(R.id.min);
        L2=(LinearLayout)findViewById(R.id.time);
        L1=(LinearLayout)findViewById(R.id.data);
        shebei=findViewById(R.id.spinner4);
        caozuo=findViewById(R.id.spinner3);
        final String [] lshebei={"请选择","风扇","插座","火焰报警","人体报警"};
        final String[][] lcaozuo={{"请选择"},{"请选择","0档","1档","2档","3档"},{"请选择","开","关"},{"请选择","开","关"},{"请选择","开","关"},{"请选择","开","关"}};

        AlertDialog.Builder dialog = new AlertDialog.Builder(TimetaskActivity.this);
        dialog.setTitle("温馨提示");
        dialog.setMessage("在选择时间的时候要选择晚于当前时间哦");
        dialog.setCancelable(false);
        dialog.setPositiveButton("知道了", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {

            }
        });
        dialog.show();

        ArrayAdapter<String> shebeiadapter=new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item,lshebei);
        shebeiadapter.setDropDownViewResource(android.R.layout.simple_list_item_single_choice);
        final ArrayAdapter<String> caozuoadapter=new ArrayAdapter<String>(this, android.R.layout.simple_spinner_item);
        caozuoadapter.setDropDownViewResource(android.R.layout.simple_list_item_single_choice);
        bt_fs=findViewById(R.id.bt_fs);
        bt_fs.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String result=getdata()+doing;
                if(result.length()!=14 | doing.equals("00")| doing.equals("01")| doing.equals("10"))
                {
                    AlertDialog.Builder dialog = new AlertDialog.Builder(TimetaskActivity.this);
                    dialog.setTitle("提醒");
                    dialog.setMessage("请确保时间 日期 设备 操作都选择了 哦");
                    dialog.setCancelable(false);
                    dialog.setPositiveButton("知道了", new DialogInterface.OnClickListener() {
                        @Override
                        public void onClick(DialogInterface dialog, int which) {

                        }
                    });
                    dialog.show();
                }else {
                    MyMqttService.publish(result, 0, "TimeTask_Request");
                }

            }
        });
        caozuo.setAdapter(caozuoadapter);
        caozuo.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                doing=shebei.getSelectedItemPosition()+""+caozuo.getSelectedItemPosition()+"";
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        shebei.setAdapter(shebeiadapter);
        shebei.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                //获取Spinner控件的适配器
                String[] cz=lcaozuo[position];
                caozuoadapter.clear();
                caozuoadapter.addAll(cz);
                caozuo.setSelection(0);

            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });
        L2.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showTimePickDlg();
            }
        });
        L1.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                showDatePickDlg();
            }
        });
    }

    protected void showDatePickDlg() {
                Calendar calendar = Calendar.getInstance();
                DatePickerDialog datePickerDialog = new DatePickerDialog(TimetaskActivity.this, new DatePickerDialog.OnDateSetListener() {


                    public void onDateSet(DatePicker view, int year, int monthOfYear, int dayOfMonth) {
                        Year.setText(year+"");
                        Month.setText(monthOfYear+1+"");
                        Day.setText(dayOfMonth+"");

                    }
                    }, calendar.get(Calendar.YEAR), calendar.get(Calendar.MONTH), calendar.get(Calendar.DAY_OF_MONTH));
                datePickerDialog.getDatePicker().setMinDate(Calendar.getInstance().getTime().getTime());
                datePickerDialog.show();

    }
    protected void showTimePickDlg() {
        Calendar c = Calendar.getInstance();

        new TimePickerDialog(TimetaskActivity.this,
                 // 绑定监听器
         new TimePickerDialog.OnTimeSetListener() {

             @Override
             public void onTimeSet(TimePicker view, int hourOfDay, int minute) {

                        Hour.setText(hourOfDay+"");
                        Min.setText(minute+"");

             }
         }
                 // 设置初始时间
                 , c.get(Calendar.HOUR_OF_DAY), c.get(Calendar.MINUTE),
                 // true表示采用24小时制
                 true).show();

    }


}
