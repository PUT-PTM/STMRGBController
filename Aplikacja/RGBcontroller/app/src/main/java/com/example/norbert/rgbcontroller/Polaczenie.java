package com.example.norbert.rgbcontroller;

import java.util.Set;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.view.Window;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.ListView;
import android.widget.Toast;

public class Polaczenie extends AppCompatActivity
{
    public ListView myListView;
    private ArrayAdapter<String> mArrayAdapter;
    BluetoothAdapter mBluetoothAdapter;

    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_polaczenie);
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        doPokaz();
        doLaczenie();
    }

    public void doPokaz()
    {
        myListView = (ListView)findViewById(R.id.listView1);
        mArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        myListView.setAdapter(mArrayAdapter);
    }

    public void doLaczenie() {
        myListView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            public void onItemClick(AdapterView<?> parent, View v, int position, long id) {
                String value = (String) myListView.getItemAtPosition(position);
                String adres = value.substring(value.length() - 17);
                Intent nowyEkran = new Intent(getApplicationContext(), Akcelerometr.class);

                nowyEkran.putExtra("adresMAC", adres); // zapisanie adresu MAC i przekazanie go do innego activity
                startActivity(nowyEkran);
                finish();

            }
        });
    }
    //-------------------------------------------------------------Pokaz powiazane
    public void powiazane(View v) {
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        if (pairedDevices.size() > 0) {
            for (BluetoothDevice device : pairedDevices) {
                mArrayAdapter.clear();
                mArrayAdapter.add(device.getName() + "\n" + device.getAddress());
            }
        } else {
            Toast.makeText(getApplicationContext(), "Brak powiązanych urządzeń!", Toast.LENGTH_LONG).show();
        }
    }
    //-------------------------------------------------------------pokaz dostepne
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
}