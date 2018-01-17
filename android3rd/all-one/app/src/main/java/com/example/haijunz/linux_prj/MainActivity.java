package com.example.haijunz.linux_prj;

import android.content.Context;
import android.content.Intent;
import android.databinding.DataBindingUtil;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import com.example.global_lib.User;
import com.example.global_lib.globalData;

import com.example.haijunz.linux_prj.databinding.ActivityMainBinding;
import com.example.lib1.lib1Activity;

// http://www.vogella.com/tutorials/AndroidDatabinding/article.html

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
      //  setContentView(R.layout.activity_main);

        ActivityMainBinding binding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        User user = new User("Lars", "Vogel ");
        binding.setUser(user);

        final Button button = findViewById(R.id.buttonapp);
        button.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                // Start NewActivity.class

              /* String CUSTOM_ACTION = "com.example.lib1.LIB1";
                Intent i = new Intent();
                i.setAction(CUSTOM_ACTION);
                startActivity(i);*/

                Intent intent = new Intent(MainActivity.this,com.example.lib1.lib1Activity.class);
                startActivity(intent);
            }
        });

      //   final TextView helloTextView = (TextView) findViewById(R.id.textView1);
        Context context = getApplicationContext();
        CharSequence text = globalData.lib1_result;
        int duration = Toast.LENGTH_SHORT;
        Toast toast = Toast.makeText(context, text, duration);
        toast.show();





    }


}
