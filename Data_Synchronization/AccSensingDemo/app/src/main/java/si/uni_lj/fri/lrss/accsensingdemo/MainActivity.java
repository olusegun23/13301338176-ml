package si.uni_lj.fri.lrss.accsensingdemo;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v4.content.LocalBroadcastManager;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    public static final String TAG = "MainActivity";

    AccBroadcastReceiver mBcastRecv;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mBcastRecv = new AccBroadcastReceiver();

        Button startButton = (Button) findViewById(R.id.start_btn);
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SensingAlarmReceiver.startAlarm(ApplicationContext.getContext());
            }
        });

        Button stopButton = (Button) findViewById(R.id.stop_btn);
        stopButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SensingAlarmReceiver.cancelAlarm(ApplicationContext.getContext());
            }
        });
    }

    @Override
    protected void onResume() {
        super.onResume();
        LocalBroadcastManager.getInstance(this).registerReceiver(mBcastRecv,
                new IntentFilter("si.uni_lj.fri.lrss.BROADCAST_RESULT"));
    }

    @Override
    protected void onPause() {
        LocalBroadcastManager.getInstance(this).unregisterReceiver(mBcastRecv);
        super.onPause();
        SensingAlarmReceiver.cancelAlarm(this);
    }

    public void recordAccData(float intensityf, float variancef, float MCRf) {
        // Handle the data here
        Log.d(TAG, "recordAccData");
    }


    public class AccBroadcastReceiver extends BroadcastReceiver {

        @Override
        public void onReceive(Context context, Intent intent) {
            float intensity = intent.getFloatExtra("mean",0);
            float variance = intent.getFloatExtra("var",0);
            float MCR = intent.getFloatExtra("MCR", 0);

            recordAccData(intensity, variance, MCR);
        }
    }
}
