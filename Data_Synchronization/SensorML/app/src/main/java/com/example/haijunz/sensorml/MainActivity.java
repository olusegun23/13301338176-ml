package com.example.haijunz.sensorml;

import android.content.ComponentName;
import android.content.Context;
import android.content.Intent;
import android.content.ServiceConnection;
import android.os.Binder;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.ImageView;
import android.widget.TextView;

import com.example.haijunz.sensorml.Env.EnvService;
import com.example.haijunz.sensorml.sensors.ARService;

import static java.lang.Thread.sleep;

public class MainActivity extends AppCompatActivity implements
        ServiceConnection  {

    public boolean isRunning = true;
    private Binder  mBinder;
    TextView mText,mText1;
    private Handler handlerRefresh;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mText = (TextView) findViewById(R.id.mText);
        mText1 = (TextView) findViewById(R.id.mText1);
        handlerRefresh = new Handler() {
            public void handleMessage(Message msg) {
                Log.v("====", "receive  from handle");
                mText.setText(mGlobal.info);
                mText1.setText(mGlobal.info1);
                super.handleMessage(msg);
            }
        };


        //  update UI
        new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    while (isRunning) {
                        sleep(200);
                        Message msg = handlerRefresh.obtainMessage();
                        msg.what = 1;
                        msg.sendToTarget();
                        Log.v("====", "send from Thread");
                        // handlerRefresh.sendEmptyMessage(0);
                    }
                } catch (InterruptedException e) {
                    e.printStackTrace();
                }
            }
        }).start();

       /* Intent intent = new Intent(this, ARService.class);
        startService(intent);
 */

    }


    @Override
    protected void onPause() {
        // TODO Auto-generated method stub
        super.onPause();
        getApplicationContext().unbindService(this);
    }

    @Override
    protected void onResume() {
        // TODO Auto-generated method stub
        super.onResume();
        // Bind to the Service

        Intent intent = new Intent(this, ARService.class);
        bindService(intent, this, Context.BIND_AUTO_CREATE);
        Intent intent1 = new Intent(this, EnvService.class);
        bindService(intent1, this, Context.BIND_AUTO_CREATE);




    }

    @Override
    public void onServiceConnected(ComponentName arg0, IBinder arg1) {
        mBinder = (Binder) arg1;
    }

    @Override
    public void onServiceDisconnected(ComponentName arg0) {
        // TODO Auto-generated method stub
    }


}
