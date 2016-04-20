package com.example.norbert.rgbcontroller;


import java.util.ArrayList;
import java.util.List;

import android.app.ListActivity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.ListView;

public class Polaczenie extends AppCompatActivity
{

    private Handler _handler = new Handler();
    private BluetoothAdapter _bluetooth = BluetoothAdapter.getDefaultAdapter();
    private List<BluetoothDevice> _devices = new ArrayList<BluetoothDevice>();

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_polaczenie);


        IntentFilter discoveryFilter = new IntentFilter(BluetoothAdapter.ACTION_DISCOVERY_FINISHED);
        registerReceiver(_discoveryReceiver, discoveryFilter);
        IntentFilter foundFilter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(_foundReceiver, foundFilter);

        _bluetooth.startDiscovery();
    }



    private BroadcastReceiver _foundReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
            _devices.add(device);
            showDevices();
        }
    };
    private BroadcastReceiver _discoveryReceiver = new BroadcastReceiver() {


        public void onReceive(Context context, Intent intent)
        {
            unregisterReceiver(_foundReceiver);
            unregisterReceiver(this);
        }
    };

    protected void showDevices()
    {
        List<String> list = new ArrayList<String>();
        if(_devices.size() > 0)
        {
            for (int i = 0, size = _devices.size(); i < size; ++i)
            {
                StringBuilder b = new StringBuilder();
                BluetoothDevice d = _devices.get(i);
                b.append(d.getAddress());
                b.append('\n');
                b.append(d.getName());
                String s = b.toString();
                list.add(s);
            }
        }
        else
            list.add("nie ma urządzeń w pobliżu!");
        final ArrayAdapter<String> adapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1, list);
        _handler.post(new Runnable() {
            public void run()
            {
                //setListAdapter(adapter);
            }
        });
    }

    protected void onListItemClick(ListView l, View v, int position, long id)
    {
        Intent result = new Intent();
        result.putExtra(BluetoothDevice.EXTRA_DEVICE, _devices.get(position));
        setResult(RESULT_OK, result);
        finish();
    }
}
