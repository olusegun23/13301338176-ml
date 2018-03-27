package com.example.haijunz.sensorml.sensors;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorManager;
import android.location.LocationManager;
import android.net.ConnectivityManager;
import android.net.NetworkInfo;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.widget.Toast;

public class ARService extends Service {
    public ARService() {
    }

    public static final int MSG_START_SENSING = 1;
    public static final int MSG_STOP_SENSING = 2;
    // Time in milliseconds
    public static final int LOCATION_MIN_TIME_CHANGE_UPDATE = 1000;
    // In meters
    public static final int LOCATION_MIN_DISTANCE_CHANGE_UPDATE = 1;

    private static boolean sHandlerRegistered = false;
    private SensorManager mSensorMgr;
    private LocationManager mLocationMgr;

    class IncomingHandler extends Handler {

        @Override
        public void handleMessage(Message msg) {

            switch (msg.what) {

                case MSG_START_SENSING:
                    break;

                case MSG_STOP_SENSING:
                    break;

                default:
                    super.handleMessage(msg);
            }
        }
    }

    /**
     * Register the listener for accelerometer and location
     */
    private void startSensors() {
        mSensorMgr = (SensorManager) getSystemService(SENSOR_SERVICE);
        mSensorMgr.registerListener(DataHandler.getHandler(),
                mSensorMgr.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
                SensorManager.SENSOR_DELAY_NORMAL);

       mLocationMgr = (LocationManager) getSystemService(LOCATION_SERVICE);
        mLocationMgr.requestLocationUpdates(LocationManager.NETWORK_PROVIDER,
                LOCATION_MIN_TIME_CHANGE_UPDATE,
                LOCATION_MIN_DISTANCE_CHANGE_UPDATE,
                DataHandler.getHandler());

        sHandlerRegistered = true;
    }

    /**
     * Unregister the listener for all sensors
     */
    private void stopSensors() {
        if (sHandlerRegistered) {
            mSensorMgr = (SensorManager) getSystemService(SENSOR_SERVICE);
            mSensorMgr.unregisterListener(DataHandler.getHandler());
        }
    }

    final Messenger mMessenger = new Messenger(new IncomingHandler());
    static final String tag = "ARService";

    /**
     * (non-Javadoc)
     *
     * @see android.app.Service#onBind(android.content.Intent)
     */
    @Override
    public IBinder onBind(Intent intent) {
        Toast.makeText(getApplicationContext(),
                "binding to sensor user-info service", Toast.LENGTH_SHORT)
                .show();

        DataHandler.getHandler().setService(this);
        boolean state = isOnline();
        // Toast.makeText(getApplicationContext(), "deviceOnline?" + state,
        // Toast.LENGTH_SHORT).show();

        DataHandler.getHandler().setDeviceConnectivity(state);
        startSensors();
        return mMessenger.getBinder();
    }

    /**
     * (non-Javadoc)
     *
     * @see android.app.Service#onUnbind(android.content.Intent)
     */
    @Override
    public boolean onUnbind(Intent intent) {
        Toast.makeText(getApplicationContext(),
                "unbinding sensor user-info service", Toast.LENGTH_SHORT)
                .show();

        stopSensors();
        DataHandler.getHandler().setService(null);
        DataHandler.clearHandler();
        return false;
    }

    /**
     *
     * @return
     */
    public boolean isOnline() {
        ConnectivityManager cm = (ConnectivityManager) getSystemService(Context.CONNECTIVITY_SERVICE);
        NetworkInfo netInfo = cm.getActiveNetworkInfo();
        if (netInfo != null && netInfo.isConnectedOrConnecting()) {
            return true;
        }

        return false;
    }

















}
