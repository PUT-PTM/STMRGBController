package com.example.norbert.akcelerometr;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity implements SensorEventListener {

    Sensor akcelerometr;
    SensorManager sm;
    TextView text;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        sm=(SensorManager)getSystemService(SENSOR_SERVICE);
        akcelerometr=sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sm.registerListener(this,akcelerometr,SensorManager.SENSOR_DELAY_NORMAL);

        text=(TextView)findViewById(R.id.textView);
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        int red,green,blue;
        //@red%green%blue$
        red =((int)(((event.values[0])+10)*12.75)%256);
        green =((int)((event.values[1]+10)*12.75)%256);
        blue =((int)((event.values[2]+10)*12.75)%256);
        String kod;
        kod = "@"+red+"%"+green+"%"+blue+"$";

        text.setText("X: " + red +"\nY: " + green + "\nZ: " + blue + "\nKod: " + kod);

    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
