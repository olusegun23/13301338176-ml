package com.example.ravikiran357.mc_project;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.TextView;
import android.widget.Toast;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.IOException;

public class ResultActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_result);
        view_result();
    }

    private void view_result() {
        File file = new File(MainActivity.FILE_PATH + "results.txt");
        StringBuilder text = new StringBuilder();
        try {
            BufferedReader br = new BufferedReader(new FileReader(file));
            String line;
            while ((line = br.readLine()) != null) {
                text.append(line);
            }
            TextView loggingText = findViewById(R.id.tses);
            loggingText.setText(text);
        }
        catch (IOException e) {
            Toast.makeText(ResultActivity.this,"Error reading file!",Toast.LENGTH_LONG).show();
            e.printStackTrace();
        }
        catch (Exception e) {
            Toast.makeText(ResultActivity.this,"File not found!",Toast.LENGTH_LONG).show();
            e.printStackTrace();
        }
    }
}
