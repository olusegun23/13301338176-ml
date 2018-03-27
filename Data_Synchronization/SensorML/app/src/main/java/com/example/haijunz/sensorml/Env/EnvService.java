package com.example.haijunz.sensorml.Env;

import android.app.Service;
import android.content.Intent;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.widget.Toast;

public class EnvService extends Service {


    public EnvService() {
    }




    EnvHandler audioHandler;

    public static String TAG = "EnvService";

    /**
     *
     * @author Ebby
     *
     */
    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                default:
                    super.handleMessage(msg);
            }
        }
    }

    final Messenger mMessenger = new Messenger(new IncomingHandler());

    /**
     *
     */
    @Override
    public IBinder onBind(Intent arg0) {
        Toast.makeText(getApplicationContext(),
                "binding to env-sensor service", Toast.LENGTH_SHORT).show();

        audioHandler = new EnvHandler();
        audioHandler.startRecording();
        audioHandler.setSensorService(this);
        return mMessenger.getBinder();
    }

    /*
     * (non-Javadoc)
     *
     * @see android.app.Service#onUnbind(android.content.Intent)
     */
    public boolean onUnbind(Intent intent) {
        Toast.makeText(getApplicationContext(), "unbinding env-sensor service",
                Toast.LENGTH_SHORT).show();

        audioHandler.stopRecording();
        audioHandler.setSensorService(null);

        return false;
    }

}
