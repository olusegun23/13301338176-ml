package com.example.ravikiran357.mc_project;

import android.Manifest;
import android.annotation.TargetApi;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.content.res.AssetManager;
import android.os.AsyncTask;
import android.os.Build;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.support.annotation.NonNull;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.HttpURLConnection;
import java.net.URL;
import java.security.SecureRandom;
import java.security.cert.CertificateException;
import java.security.cert.X509Certificate;

import javax.net.ssl.HostnameVerifier;
import javax.net.ssl.HttpsURLConnection;
import javax.net.ssl.SSLContext;
import javax.net.ssl.SSLSession;
import javax.net.ssl.TrustManager;
import javax.net.ssl.X509TrustManager;

public class MainActivity extends AppCompatActivity {

    private static final int PERMISSION_STORAGE = 1;
    public static final String FILE_PATH = Environment.getExternalStorageDirectory() +
            File.separator + "Android/Data/CSE535_Project_Group11/";
    public static final String LOG_FILE_PATH = FILE_PATH + "LastRun.log";
    public static final String SERVER_REQUEST_URL = "http://10.208.98.126/test";
    public static final String INPUT_FILE_NAME = "input";

    private ProgressDialog progressDialog;
    private ProgressDialog progressDialog2;
    private Handler mHandler;
    private Spinner MLSpinner;
    private int percent = -1;
    private String classifier = "";
    private boolean res;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
            getPermissions();
        } else {
            setup_UI();
        }
    }

    @TargetApi(Build.VERSION_CODES.M)
    private void getPermissions() {
        int permissionCheck1 = this.checkSelfPermission(Manifest.permission.WRITE_EXTERNAL_STORAGE);
        if (permissionCheck1 == PackageManager.PERMISSION_GRANTED ) {
            setup_UI();
        } else {
            requestPermissions(new String[]{Manifest.permission.WRITE_EXTERNAL_STORAGE},
                    PERMISSION_STORAGE);
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String permissions[],
                                           @NonNull int[] grantResults) {
        switch (requestCode) {
            case PERMISSION_STORAGE: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 &&
                        grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                    setup_UI();
                } else {
                    Toast.makeText(this, "Please restart the app",
                            Toast.LENGTH_SHORT).show();
                }
            }
        }
    }

    private void setup_UI() {
        File folder = new File(FILE_PATH);
        if (!folder.exists()) {
            folder.mkdir();
        }
        addListenerOnTrain();
        addListnerOnPercent();
        addListenerOnLog();
        addListenerOnClearLog();
        addListenerOnClearResult();
        addListenerOnRes();
        addListenerOnEval();
    }

    private void addListenerOnTrain() {
        MLSpinner = findViewById(R.id.MLspinner);
        MLSpinner.setSelection(0);
        Button trainButton = findViewById(R.id.btnRunAlgo);
        trainButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Spinner percentET = findViewById(R.id.percentET);
                getTrainedModelFromServer(SERVER_REQUEST_URL, percentET.getSelectedItem().
                        toString(), String.valueOf(MLSpinner.getSelectedItem()));
            }
        });
    }

    private void addListnerOnPercent() {
        Spinner percentET = findViewById(R.id.percentET);
        Integer[] numbers = new Integer[99];
        for (int i = 1; i < 100; i++)
            numbers[i-1] = i;
        ArrayAdapter<Integer> adapter = new ArrayAdapter<Integer>(MainActivity.this,
                android.R.layout.simple_spinner_item, numbers);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        percentET.setAdapter(adapter);
        percentET.setSelection(79);
    }

    private void addListenerOnLog() {
        Button logButton = findViewById(R.id.logBtn);
        // if no log file found then disable the button, spinner otherwise populate it with the file names
        logButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, LoggingActivity.class);
                startActivity(intent);
            }
        });
    }

    private void addListenerOnClearLog() {
        Button logButton = findViewById(R.id.logClearBtn);
        // if no log file found then disable the button, spinner otherwise populate it with the file names
        logButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                File file = new File(LOG_FILE_PATH);
                if (file.exists()) {
                    if (file.delete())
                        Toast.makeText(MainActivity.this,"Deleted log file!",
                                Toast.LENGTH_SHORT).show();
                    else
                        Toast.makeText(MainActivity.this,"Could not clear the log file!",
                                Toast.LENGTH_SHORT).show();
                } else {
                    Toast.makeText(MainActivity.this,"Log file doesn't exist!",
                            Toast.LENGTH_SHORT).show();
                }
            }
        });
    }

    private void addListenerOnClearResult() {
        Button logButton = findViewById(R.id.resClearBtn);
        // if no log file found then disable the button, spinner otherwise populate it with the file names
        logButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                File file = new File(FILE_PATH);
                if (file.isDirectory()) {
                    for (File child : file.listFiles()) {
                        child.delete();
                    }
                }
                Button evalButton = findViewById(R.id.btnEvalAlgo);
                evalButton.setEnabled(false);
                Button resButton = findViewById(R.id.resBtn);
                resButton.setEnabled(false);
                Toast.makeText(MainActivity.this, "Deleted result model!",
                        Toast.LENGTH_SHORT).show();
            }
        });
    }


    private void addListenerOnRes() {
        Button resButton = findViewById(R.id.resBtn);
        // if no log file found then disable the button, spinner otherwise populate it with the file names
        resButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Intent intent = new Intent(MainActivity.this, ResultActivity.class);
                startActivity(intent);
            }
        });
    }

    private void addListenerOnEval() {
        Button resButton = findViewById(R.id.btnEvalAlgo);
        // if no log file found then disable the button, spinner otherwise populate it with the file names
        resButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
//                EvalTask evalTask = new EvalTask(MainActivity.this);
                progressDialog2 = new ProgressDialog(MainActivity.this);
                progressDialog2.setTitle("Testing...");
                progressDialog2.setMessage("Evaluating " + classifier + " model");
                progressDialog2.setIndeterminate(true);
                progressDialog2.setCancelable(false);
                progressDialog2.show();

                mHandler = new Handler();// This statement is to be called by the main thread
                Thread t = new Thread(
                    new Runnable(){
                        public void run() {
                            MLTestDriver mlTestDriver = new MLTestDriver();
                            res = (percent != -1 && mlTestDriver.test(percent, classifier,
                                    getInputFile()));
                            mHandler.post(new Runnable(){
                                public void run() {
                                    progressDialog2.dismiss();
                                    if (res) {
                                        Toast.makeText(MainActivity.this, "Testing complete",
                                                Toast.LENGTH_SHORT).show();
                                        Button resButton = findViewById(R.id.resBtn);
                                        resButton.setEnabled(true);
                                        Button evalButton = findViewById(R.id.btnEvalAlgo);
                                        evalButton.setEnabled(false);
                                    } else {
                                        Toast.makeText(MainActivity.this, "Testing failed",
                                                Toast.LENGTH_SHORT).show();
                                    }
                                }
                            });
                        }
                    }
                );
                t.start();

//                MLTestDriver mlTestDriver = new MLTestDriver();
//                if (percent != -1 && mlTestDriver.test(percent, classifier, getInputFile())) {
//                    Toast.makeText(MainActivity.this, "Testing complete",
//                            Toast.LENGTH_SHORT).show();
//                    Button resButton = findViewById(R.id.resBtn);
//                    resButton.setEnabled(true);
//                } else {
//                    Toast.makeText(MainActivity.this, "Testing failed",
//                            Toast.LENGTH_SHORT).show();
//                }
//                progressDialog2.dismiss();
            }
        });
    }

    @NonNull
    private String storeInputFile() {
        File f = new File(getCacheDir()+"/" + INPUT_FILE_NAME + ".csv");
        if (!f.exists())
            try {
            InputStream is = getAssets().open(INPUT_FILE_NAME + ".csv");
            int size = is.available();
            byte[] buffer = new byte[size];
            is.read(buffer);
            is.close();
            FileOutputStream fos = new FileOutputStream(f);
            fos.write(buffer);
            fos.close();
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        return f.getPath();
    }

    @NonNull
    private String getInputFile() {
        AssetManager assetManager = getAssets();
        String[] files = null;
        try {
            files = assetManager.list("");
        } catch (IOException e) {
            Log.e("tag", "Failed to get asset file list.", e);
        }
        if (files != null) {
            for (String filename : files) {
                if (filename.contains("t_input")) {
                    return storeInputFile();
                }
            }
        }
        return "";
    }

    private void getTrainedModelFromServer(String serverRequestURL, String percent,
                                           String classifierName) {
        DownloadTask downloadTask = new DownloadTask(MainActivity.this);
        progressDialog = new ProgressDialog(MainActivity.this);
        progressDialog.setTitle("Training...");
        progressDialog.setMessage("Downloading model from server");
        progressDialog.setIndeterminate(true);
        progressDialog.setCancelable(false);
        progressDialog.setProgressStyle(ProgressDialog.STYLE_HORIZONTAL);
        downloadTask.execute(serverRequestURL, percent, classifierName);
    }

    public class NullHostNameVerifier implements HostnameVerifier {
        @Override
        public boolean verify(String hostname, SSLSession session) {
            Log.i("RestUtilImpl", "Approving certificate for " + hostname);
            return true;
        }
    }

    private class DownloadTask extends AsyncTask<String, Integer, String> {
        private Context context;

        DownloadTask(Context context) {
            this.context = context;
        }

        @Override
        protected String doInBackground(String... params) {
            InputStream input = null;
            OutputStream output = null;
            HttpURLConnection connection = null;
            TrustManager[] trustAllCerts = new TrustManager[] { new X509TrustManager() {
                public X509Certificate[] getAcceptedIssuers() {return null;}
                @Override
                public void checkClientTrusted(X509Certificate[] arg0, String arg1) throws CertificateException {}
                @Override
                public void checkServerTrusted(X509Certificate[] arg0, String arg1) throws CertificateException {}
            } };

            try {
                HttpsURLConnection.setDefaultHostnameVerifier(new NullHostNameVerifier());
                SSLContext sc = SSLContext.getInstance("TLS");
                sc.init(null, trustAllCerts, new SecureRandom());
                HttpsURLConnection.setDefaultSSLSocketFactory(sc.getSocketFactory());

                percent = Integer.valueOf(params[1]);
                classifier = params[2];
                URL url = new URL(params[0] + "?percent=" + percent + "&classifier=" +
                        classifier);
//                 connection = (HttpsURLConnection) url.openConnection();
                connection = (HttpURLConnection) url.openConnection();
                connection.setInstanceFollowRedirects(true);
                connection.setRequestMethod("GET");
                connection.connect();
                if (connection.getResponseCode() != HttpsURLConnection.HTTP_OK )
                    return "Server returned HTTP " + connection.getResponseCode() + " " +
                            connection.getResponseMessage();
                int fileLength = connection.getContentLength();
                input = connection.getInputStream();
                output = new FileOutputStream(FILE_PATH + classifier + ".model");
                byte data[] = new byte[4096];
                long total = 0;
                int count;
                while ((count = input.read(data)) != -1) {
                    // allow canceling with back button
                    if (isCancelled()) {
                        input.close();
                        return null;
                    }
                    total += count;
                    // publishing the progress....
                    if (fileLength > 0) // only if total length is known
                        publishProgress((int) (total * 100 / fileLength));
                    output.write(data, 0, count);
                }
            } catch (Exception e) {
                return e.toString();
            } finally {
                try {
                    if (input != null)
                        input.close();
                    if (output != null)
                        output.close();
                } catch (IOException e) {
                    Log.d("exp-AfterDownload","Check here");
                }
                if (connection != null)
                    connection.disconnect();
            }
            return null;
        }


        @Override
        protected void onPreExecute() {
            progressDialog.show();
            super.onPreExecute();
        }

        @Override
        protected void onProgressUpdate(Integer... progress) {
            super.onProgressUpdate(progress);
            // if we get here, length is known, now set indeterminate to false
            progressDialog.setIndeterminate(false);
            progressDialog.setMax(100);
            progressDialog.setProgress(progress[0]);
        }

        @Override
        protected void onPostExecute(String result) {
            progressDialog.dismiss();
            if (result != null) {
                Toast.makeText(context,"Download error: " + result,
                        Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(context,"Downloaded Trained Model",
                        Toast.LENGTH_SHORT).show();
                Button evalButton = findViewById(R.id.btnEvalAlgo);
                evalButton.setEnabled(true);
            }
        }
    }

    private class EvalTask extends AsyncTask<String, Integer, String> {
        private Context context;
        int percent = -1;
        String classifier = "";

        EvalTask(Context context) {
            this.context = context;
        }

        @Override
        protected String doInBackground(String... params) {
            percent = Integer.valueOf(params[1]);
            classifier = params[2];
            MLTestDriver mlTestDriver = new MLTestDriver();
            if (percent != -1 && mlTestDriver.test(percent, classifier, getInputFile())) {
                return null;
            }
            return "fail";
        }

        @Override
        protected void onPreExecute() {
            progressDialog2.show();
            super.onPreExecute();
        }

        @Override
        protected void onPostExecute(String result) {
            progressDialog2.dismiss();
            if (result != null) {
                Toast.makeText(context,"Testing failed" + result,
                        Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(context,"Evaluation complete",
                        Toast.LENGTH_SHORT).show();
                Button resButton = findViewById(R.id.resBtn);
                resButton.setEnabled(true);
            }
        }
    }
}
