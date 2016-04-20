package com.example.norbert.rgbcontroller;

import android.content.Intent;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;

public class Akcelerometr extends AppCompatActivity implements SensorEventListener{
    Sensor akcelerometr;
    SensorManager sm;
    TextView text;
    ImageView background;
    private static final int REQUEST_DISCOVERY = 0x1;;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_akcelerometr);
        sm=(SensorManager)getSystemService(SENSOR_SERVICE);
        akcelerometr=sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sm.registerListener(this,akcelerometr,SensorManager.SENSOR_DELAY_NORMAL);
        text=(TextView)findViewById(R.id.textView);
        background = (ImageView)findViewById(R.id.imageView);
       startActivity(new Intent(Akcelerometr.this,Polaczenie.class));

    }


    public void onSensorChanged(SensorEvent event) {
        int red,green,blue;
        //$a%123%123%123#
        red =((int)(((event.values[0])+10)*12.75)%256);
        green =((int)((event.values[1]+10)*12.75)%256);
        blue =((int)((event.values[2]+10)*12.75)%256);
        String kod;
        kod = "$a%"+red+"%"+green+"%"+blue+"#";
        text.setText("X: " + red +"\nY: " + green + "\nZ: " + blue + "\nKod: " + kod);
        background.setBackgroundColor(Color.rgb(red, green, blue));
    }
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }

    public void wstecz(View v)
    {
        startActivity(new Intent(Akcelerometr.this, MainActivity.class));
    }
}
