package si.uni_lj.fri.lrss.accsensingdemo;

import android.app.Application;
import android.content.Context;

/**
 * Created by veljko on 01/11/15.
 */
public class ApplicationContext extends Application {

    private static Context instance;

    @Override
    public void onCreate() {
        super.onCreate();
        instance = getApplicationContext();
    }

    public static Context getContext()
    {
        return instance;
    }

}
