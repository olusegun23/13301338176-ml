package com.example.lib1;

import android.content.Intent;
import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.example.global_lib.User;
import com.example.global_lib.globalData;

public class lib1Activity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_lib1);

/*
        // Get the intent that started this activity
        Intent intent = getIntent();
        Uri data = intent.getData();

        // Figure out what to do based on the intent type
        if (intent.getType().indexOf("image/") != -1) {
            // Handle intents with image data ...
        } else if (intent.getType().equals("text/plain")) {
            // Handle intents with text ...

        }

*/
            final Button button = findViewById(R.id.buttonlib1);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Start NewActivity.class
            /*    Intent intent = new Intent(lib1Activity.this,com.example.haijunz.linux_prj.MainActivity.class);
                startActivity(intent);*/
               User.firstName="LIB1 user";
               User.lastName="LIB1 last";

            }
        });








    }
}
