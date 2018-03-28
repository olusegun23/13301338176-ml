package com.example.ravikiran357.mc_project;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;

public class LoggingActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_logging);
        view_logs();
    }

    private void view_logs() {
        File file = new File(MainActivity.FILE_PATH + "LastRun.log");
        StringBuilder text = new StringBuilder();
        try {
            BufferedReader br = new BufferedReader(new FileReader(file));
            String line;
            while ((line = br.readLine()) != null) {
                text.append(line);
                text.append('\n');
            }
        } catch (IOException e) {
            Toast.makeText(LoggingActivity.this,"No log found!",Toast.LENGTH_LONG).show();
            e.printStackTrace();
        } catch (Exception e) {
            Toast.makeText(LoggingActivity.this,"File not found!",Toast.LENGTH_LONG).show();
            e.printStackTrace();
        }
        TextView loggingText = findViewById(R.id.LOGGING_TV);
        ScrollView scrollView = findViewById(R.id.SCROLLER_SV);
        scrollView.fullScroll(View.FOCUS_DOWN);
        loggingText.setText(text);
    }
}
