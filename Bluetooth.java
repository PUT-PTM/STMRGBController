package com.example.norbert.bt;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;

public class Bluetooth extends AppCompatActivity {
    public ListView myListView;
    private ArrayAdapter<String> mArrayAdapter;
    BluetoothAdapter mBluetoothAdapter;
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805f9b34fb");


    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bluetooth);
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        doPokaz();
        doLaczenie();
    }


    //-------------------------------------------------------------------------uruchamianie BT

    public void wlacz(View v) {
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, 1);
        }
        else {
            Toast.makeText(getApplicationContext(),"Bluetooth jest już uruchomione", Toast.LENGTH_LONG).show();
        }
    }

    //-------------------------------------------------------------------------wyszukiwanie urządzeń
    public void doPokaz()
    {
        myListView = (ListView)findViewById(R.id.listView);
        mArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        myListView.setAdapter(mArrayAdapter);
    }
    final BroadcastReceiver mReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                mArrayAdapter.add(device.getName() + "\n" + device.getAddress());
                mArrayAdapter.notifyDataSetChanged();
            }
        }
    };

    public void pokaz(View v) {
        if (mBluetoothAdapter.isDiscovering()) {
            mBluetoothAdapter.cancelDiscovery();
        }
        else {
            mArrayAdapter.clear();
            mBluetoothAdapter.startDiscovery();

            registerReceiver(mReceiver, new IntentFilter(BluetoothDevice.ACTION_FOUND));
        }
    }
    //-------------------------------------------------------------------------łączenie

    public void doLaczenie() {
        myListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                String value = (String) myListView.getItemAtPosition(position);
                String adres = value.substring(value.length() - 17);
                Toast.makeText(getApplicationContext(),""+adres, Toast.LENGTH_LONG).show();
            }
        });
    }
    //-------------------------------------------------------------------------pisanie
    public void write(byte[] bytes) {
        try {
            mmOutStream.write(bytes);
        } catch (IOException e) { }
    }

    //-------------------------------------------------------------------------powrot do głównego
    public void wstecz(View v) {
        startActivity(new Intent(Bluetooth.this, Start.class));
    }
};

