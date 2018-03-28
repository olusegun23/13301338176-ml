package si.uni_lj.fri.lrss.accsensingdemo;

import android.annotation.SuppressLint;
import android.app.AlarmManager;
import android.app.PendingIntent;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.Build;
import android.os.SystemClock;
import android.util.Log;
import android.widget.Toast;

public class SensingAlarmReceiver extends BroadcastReceiver {

    public static final String TAG = "SensingAlarmReceiver";

    public static final String ALARM_ACTION = "si.uni_lj.fri.lrss.ALARM_ACTION";


    @Override
    public void onReceive(Context context, Intent intent) {
        Log.d(TAG, "onReceive");

        Toast.makeText(ApplicationContext.getContext(), "Alarm received", Toast.LENGTH_LONG).show();

        Intent uploadIntent = new Intent(ApplicationContext.getContext(), AccSenseService.class);
        ApplicationContext.getContext().startService(uploadIntent);

        startAlarm(ApplicationContext.getContext());
    }

    public static void cancelAlarm(Context a_context)
    {
        Intent intent = new Intent(ALARM_ACTION);
        PendingIntent pi = PendingIntent.getBroadcast(ApplicationContext.getContext(), 1, intent, 0);
        AlarmManager alarmManager = (AlarmManager) a_context.getSystemService(Context.ALARM_SERVICE);
        alarmManager.cancel(pi);
    }

    @SuppressLint("NewApi")
    public static synchronized void startAlarm(Context context) {

        Log.d(TAG, "startAlarm");

        AlarmManager am = (AlarmManager) context.getSystemService(Context.ALARM_SERVICE);
        Intent intent = new Intent(ALARM_ACTION);
        PendingIntent pi = PendingIntent.getBroadcast(context, 1, intent, 0);

        long periodMillis = 5*1000;

        int type = AlarmManager.ELAPSED_REALTIME_WAKEUP;
        long fireTime = SystemClock.elapsedRealtime() + periodMillis;

        if (Build.VERSION.SDK_INT >= 19)
            am.setExact(type,fireTime,pi);
        else
            am.set(type,fireTime,pi);

    }
}
