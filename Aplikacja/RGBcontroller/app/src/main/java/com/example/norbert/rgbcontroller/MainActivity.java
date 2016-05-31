package com.example.norbert.rgbcontroller;

import android.bluetooth.BluetoothAdapter;
import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.view.Window;
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
       if (!mBluetoothAdapter.isEnabled()) {
           Toast.makeText(getApplicationContext(), "Najpierw włącz BT", Toast.LENGTH_LONG).show();
       }
        else {
           startActivity(new Intent(MainActivity.this, Polaczenie.class));
           finish();
       }
    }
    public void wylacz(View v)
    {
        finish();
    }
}
