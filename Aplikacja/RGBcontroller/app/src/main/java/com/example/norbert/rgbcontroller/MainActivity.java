package com.example.norbert.rgbcontroller;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    BluetoothAdapter mBluetoothAdapter;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    }
    public void wlaczBT(View v)
    {
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, 1);
        }
        else {
            Toast.makeText(getApplicationContext(), "Bluetooth jest juz uruchomione", Toast.LENGTH_LONG).show();
        }
    }

    public void akcelerometr(View v)
    {
       startActivity(new Intent(MainActivity.this,Akcelerometr.class));
    }

    public void wylacz(View v)
    {
        //wylaczanie BT
        System.exit(0);
    }
}
