package com.patrick115.tamz21;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.google.android.material.snackbar.Snackbar;

public class MainActivity extends AppCompatActivity {

    Button testButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        testButton = findViewById(R.id.buttonTest);
        testButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                Toast.makeText(MainActivity.this, "TAMZ II :)", Toast.LENGTH_SHORT).show();
                Snackbar.make(v, "TAMZ2", Snackbar.LENGTH_LONG).setAction("Action", null).show();

                Intent myIntent = new Intent(MainActivity.this, MainActivity2.class);
                myIntent.putExtra("loginData", "patrick115");
                startActivity(myIntent);
            }
        });
    }

    public void myAction(View view) {
        TextView textView = findViewById(R.id.textView);

        if (view.getId() == R.id.button) {
            Toast.makeText(this, "Ahoj TAMZ II", Toast.LENGTH_SHORT).show();
            textView.setText("AHOJ TAMZ II");
            textView.setTextSize(40);
        } else {
            Toast.makeText(this, "Papa TAMZ II", Toast.LENGTH_LONG).show();
            textView.setText("PAPA TAMZ II");
            textView.setTextSize(15);
        }
    }
}