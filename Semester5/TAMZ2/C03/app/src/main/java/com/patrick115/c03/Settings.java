package com.patrick115.c03;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.view.View;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class Settings extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_settings);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });
    }

    public void resetShaderPreferences(View view) {
        SharedPreferences settings = getSharedPreferences("settings", Context.MODE_PRIVATE);
        SharedPreferences.Editor settingsEditor = settings.edit();
        settingsEditor.clear();
        settingsEditor.apply();
        SharedPreferences history = getSharedPreferences("history", Context.MODE_PRIVATE);
        SharedPreferences.Editor historyEditor = history.edit();
        historyEditor.clear();
        historyEditor.apply();

        Toast.makeText(this, "SharedPreferences cleared", Toast.LENGTH_SHORT).show();
        Intent returnIntent = new Intent();
        setResult(Activity.RESULT_OK,returnIntent);
        finish();
    }

    private void setColors(int primary, int secondary) {
        SharedPreferences settings = getSharedPreferences("settings", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = settings.edit();
        editor.putInt("primary", primary);
        editor.putInt("secondary", secondary);
        editor.apply();

        Intent returnIntent = new Intent();
        setResult(Activity.RESULT_OK,returnIntent);
        finish();
    }
    public void setColorBlue(View view) {
        setColors(Color.parseColor("#8BE8FE"), Color.parseColor("#00FEFE"));
        Toast.makeText(this, "Set graph color to Blue", Toast.LENGTH_SHORT).show();
    }

    public void setColorGreen(View view) {
        setColors(Color.parseColor("#3cb514"), Color.parseColor("#49e615"));
        Toast.makeText(this, "Set graph color to Green", Toast.LENGTH_SHORT).show();
    }

    public void setColorRed(View view) {
        setColors(Color.parseColor("#e61523"), Color.parseColor("#b51b25"));
        Toast.makeText(this, "Set graph color to Red", Toast.LENGTH_SHORT).show();
    }

    public void setColorPurple(View view) {
        setColors(Color.parseColor("#7f16db"), Color.parseColor("#ad16db"));
        Toast.makeText(this, "Set graph color to Purple", Toast.LENGTH_SHORT).show();
    }
}