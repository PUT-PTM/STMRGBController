package com.example.norbert.rgbcontroller;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.graphics.Color;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.Build;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.ImageView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.OutputStream;
import java.lang.reflect.Method;
import java.util.UUID;

public class Akcelerometr extends AppCompatActivity implements SensorEventListener {


    String adres;
    BluetoothAdapter mBluetoothAdapter;
    private BluetoothSocket btSocket = null;
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private ConnectedThread mConnectedThread;

    Sensor akcelerometr;
    SensorManager sm;
    TextView text;
    ImageView background;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_akcelerometr);


        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        adres();
        doPolacz();
        doAkcelerometr();
    }

    //-----------------------------------------------------------------------polaczenie
    public void adres()
    {
        Intent i = getIntent();
        adres = i.getStringExtra("adresMAC");
    }

    public void doPolacz()
    {
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(adres);
        try {
            btSocket = createBluetoothSocket(device);
        } catch (IOException e) {
        }

        try {
            btSocket.connect();
            Toast.makeText(getApplicationContext(), "polaczono z: " + adres, Toast.LENGTH_LONG).show();
        } catch (IOException e) {
            try {
                Toast.makeText(getApplicationContext(), "rozlaczono z: " + adres, Toast.LENGTH_LONG).show();
                btSocket.close();
            } catch (IOException e2) {
            }
        }
        mConnectedThread = new ConnectedThread(btSocket);
        mConnectedThread.start();
    }

    public BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {
        if (Build.VERSION.SDK_INT >= 10) {
            try {
                final Method m = device.getClass().getMethod("createInsecureRfcommSocketToServiceRecord", new Class[] {UUID.class});
                return (BluetoothSocket) m.invoke(device, MY_UUID);
            } catch (Exception e) {

            }
        }
        return device.createRfcommSocketToServiceRecord(MY_UUID);
    }

    public class ConnectedThread extends Thread {
        public final OutputStream mmOutStream;

        public ConnectedThread(BluetoothSocket socket) {
            OutputStream tmpOut = null;


            try {
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }


            mmOutStream = tmpOut;
        }


        public void write(View v) {

            String wiadomosc = "witaj";
            byte[] msgBuffer = wiadomosc.getBytes();
            try {
                mmOutStream.write(msgBuffer); // wysyłanie danych
            } catch (IOException e) {
            }
        }
    }

    //-----------------------------------------------------------------------Akcelerometr

    public void doAkcelerometr()
    {
        sm=(SensorManager)getSystemService(SENSOR_SERVICE);
        akcelerometr=sm.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        sm.registerListener(this,akcelerometr,SensorManager.SENSOR_DELAY_NORMAL);
        text=(TextView)findViewById(R.id.textView);
        background = (ImageView)findViewById(R.id.imageView);
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

    //-----------------------------------------------------------------------wstecz
    public void wstecz(View v)
    {

        startActivity(new Intent(Akcelerometr.this, MainActivity.class));
        try {
            btSocket.close();
            finish();
            Toast.makeText(getApplicationContext(), "rozlaczono", Toast.LENGTH_LONG).show();
            return ;
        } catch (IOException e) {
        }
    }
}
