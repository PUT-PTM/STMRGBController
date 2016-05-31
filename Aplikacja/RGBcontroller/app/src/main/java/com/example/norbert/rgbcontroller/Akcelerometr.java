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
import android.view.Window;
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
    public OutputStream mmOutStream = null;

    Sensor akcelerometr;
    SensorManager sm;
    TextView text;
    ImageView background;
    boolean pozwolenie = false;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_akcelerometr);

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        adres();
        doPolacz();
        doAkcelerometr();
    }

    public void adres() //odbieranie adresu MAC urządzenia
    {
        Intent i = getIntent();
        adres = i.getStringExtra("adresMAC");
    }

    //-----------------------------------------------------------------------polaczenie

    public void doPolacz() // do onCreate
    {
        BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(adres);
        try {
            btSocket = createBluetoothSocket(device); // tworzenie socketa
        } catch (IOException e) {
        }

        try {
            btSocket.connect(); // łączenie z socketem
            Toast.makeText(getApplicationContext(), "polaczono z: " + adres, Toast.LENGTH_SHORT).show();

        } catch (IOException e) {
            try {
                Toast.makeText(getApplicationContext(), "rozlaczono z: " + adres, Toast.LENGTH_SHORT).show();
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
        public ConnectedThread(BluetoothSocket socket) {
            OutputStream tmpOut = null;

            try {
                tmpOut = socket.getOutputStream();
            } catch (IOException e) { }

            mmOutStream = tmpOut;
            wyslij("$hi#"); // komenda mówiąca płytce ze ma zacząć sterować diodami
            pozwolenie = true;
        }
    }

    public void wyslij(String wiadomosc) {

        byte[] msgBuffer = wiadomosc.getBytes(); // konwersja na bity
        try {
            mmOutStream.write(msgBuffer); // wysyłanie danych
            } catch (IOException e) {
            }
        }


    //-----------------------------------------------------------------------Akcelerometr

    public void doAkcelerometr() // do onCreate
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
        red =((int)(((event.values[0])+10)*12.75)%256); // odczytanie wartości z osi X i przekonwertowanie jej na int-a z zakresu 0-255
        green =((int)((event.values[1]+10)*12.75)%256); // odczytanie wartości z osi Y i przekonwertowanie jej na int-a z zakresu 0-255
        blue =((int)((event.values[2]+10)*12.75)%256);  // odczytanie wartości z osi Z i przekonwertowanie jej na int-a z zakresu 0-255
        String kod;
        kod = "$a%"+red+"%"+green+"%"+blue+"#"; // ustalony kod
        if (pozwolenie == true)
            wyslij(kod); //wysylanie ustalonego kodu
        text.setText("X: " + red +"\nY: " + green + "\nZ: " + blue + "\nKod: " + kod);
        background.setBackgroundColor(Color.rgb(red, green, blue));
    }
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
    }

    //-----------------------------------------------------------------------wstecz
    public void wstecz(View v)
    {
        pozwolenie = false;
        wyslij("$bye#"); // komenda kończąca wysyłanie

        startActivity(new Intent(Akcelerometr.this, MainActivity.class));
        try {
            btSocket.close();
            finish();
            Toast.makeText(getApplicationContext(), "rozlaczono", Toast.LENGTH_SHORT).show();
            return ;
        } catch (IOException e) {
        }
    }
}
