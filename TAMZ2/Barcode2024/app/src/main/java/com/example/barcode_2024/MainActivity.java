package com.example.barcode_2024;

import static android.Manifest.permission.READ_EXTERNAL_STORAGE;
import static android.Manifest.permission.WRITE_EXTERNAL_STORAGE;

import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.google.android.material.textfield.TextInputEditText;

public class MainActivity extends AppCompatActivity {

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

        TextView errorText = findViewById(R.id.errorText);
        BarcodeView barcode = findViewById(R.id.barcodeView);
        TextInputEditText input = findViewById(R.id.codeInput);
        
        Button button = findViewById(R.id.button);
        button.setOnClickListener(v -> {
            String text = input.getText().toString();
            Log.i("IDK", String.valueOf(text.length()));
            if (text.length() < 11 || text.length() > 12) {
                errorText.setText("Set text, which will be 11 or 12 digits in length");
                return;
            }

            errorText.setText("");

            int lastDigit = barcode.setCode(text);

            if (text.length() == 11) {
                errorText.setText("Last digit was autocompleted to: " + lastDigit);
            } else if (lastDigit != Character.getNumericValue(text.charAt(11))) {
                errorText.setText("You have entered the wrong last digit. It should be: " + lastDigit);
            }
        });
        
        Button save = findViewById(R.id.saveButton);
        save.setOnClickListener(v -> {
            if (!barcode.saveToFile()) {
                Toast.makeText(this, "Saving file failed :(", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(this, "File was saved", Toast.LENGTH_SHORT).show();
            }
        });

    }
}