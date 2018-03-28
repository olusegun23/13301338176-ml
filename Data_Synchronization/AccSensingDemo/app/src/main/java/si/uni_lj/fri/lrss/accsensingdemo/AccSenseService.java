package si.uni_lj.fri.lrss.accsensingdemo;

import android.app.IntentService;
import android.content.Intent;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.support.v4.content.LocalBroadcastManager;
import android.util.Log;

import java.util.ArrayList;

/**
 * Created by veljko on 01/11/15.
 */
public class AccSenseService extends IntentService implements SensorEventListener {

    public static final String TAG = "AccSenseService";

    private SensorManager mSensorManager;

    protected ArrayList<float[]> mSensorReadings;

    public AccSenseService() {
        super(TAG);
        Log.d(TAG, "AccSenseService");
    }

    public AccSenseService(String name) {
        super(name);
        Log.d(TAG, "AccSenseService");
    }

    @Override
    public void onCreate() {
        super.onCreate();
        mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
        Log.d(TAG, "onCreate");
    }


    @Override
    protected void onHandleIntent(Intent intent) {
        Log.d(TAG, "onHandleIntent");

        if (mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER) != null) {
            mSensorManager.registerListener(this,
                    mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
                    SensorManager.SENSOR_DELAY_NORMAL);
            mSensorReadings = new ArrayList<>();
        }
    }


    private void getAccelerometer(SensorEvent event) {

        float[] values = event.values;
        float x = values[0];
        float y = values[1];
        float z = values[2];

        mSensorReadings.add(new float[]{x,y,z});
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        //Log.d(TAG, "onSensorChanged");
        getAccelerometer(event);
        if (mSensorReadings.size() > 49) {
            mSensorManager.unregisterListener(this);

            float mean = 0;
            float variance = 0;
            float MCR = 0;

            float totalIntensity = 0;
            float intensityList[] = new float[mSensorReadings.size()];

            for (int i=0; i<mSensorReadings.size(); i++) {
                float[] sample = mSensorReadings.get(i);
                float intensity = (float) Math.sqrt(Math.pow(sample[0], 2) + Math.pow(sample[1], 2) + Math.pow(sample[2], 2));
                intensityList[i] = intensity;
                totalIntensity += intensity;
            }
            if (mSensorReadings.size() > 0) {
                mean = totalIntensity/mSensorReadings.size();

                for (int i=0; i<mSensorReadings.size(); i++){
                    variance += Math.pow(intensityList[i] - mean, 2);
                    if (i>0) {
                        if ((intensityList[i] - mean) * (intensityList[i-1] - mean) < 0){
                            MCR++;
                        }
                    }
                }

                variance = variance/mSensorReadings.size();

                if (mSensorReadings.size() > 1) {
                    MCR = MCR/(mSensorReadings.size()-1);
                }
            }

            Intent localIntent = new Intent("si.uni_lj.fri.lrss.BROADCAST_RESULT");
            localIntent.putExtra("mean", mean);
            localIntent.putExtra("variance", variance);
            localIntent.putExtra("MCR", MCR);
            LocalBroadcastManager.getInstance(this).sendBroadcast(localIntent);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {

    }
}
