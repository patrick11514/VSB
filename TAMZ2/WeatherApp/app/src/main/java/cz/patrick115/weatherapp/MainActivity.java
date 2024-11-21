package cz.patrick115.weatherapp;

import android.content.Intent;
import android.media.Image;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.android.volley.Request;
import com.android.volley.RequestQueue;
import com.android.volley.Response;
import com.android.volley.VolleyError;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.google.android.material.textfield.TextInputEditText;
import com.google.gson.Gson;

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

        ((CustomApp) this.getApplication()).queue = Volley.newRequestQueue(this);

        TextView temp = findViewById(R.id.temp);
        TextView desc = findViewById(R.id.desc);
        ImageView icon = findViewById(R.id.icon);

        TextInputEditText input = findViewById(R.id.town);

        Button search = findViewById(R.id.button);
        search.setOnClickListener(v -> {
            String URL = "https://api.openweathermap.org/data/2.5/weather?q=" + input.getText().toString() + ",CZ&appid=" + ((CustomApp) this.getApplication()).API_KEY + "&lang=cz&units=metric";

            StringRequest req = new StringRequest(Request.Method.GET, URL, (Response.Listener<String>) response -> {
                Gson gson = new Gson();
                ApiResponse apiResponse = gson.fromJson(response, ApiResponse.class);
                temp.setText(apiResponse.main.temp + "°C");
                desc.setText(String.valueOf(apiResponse.weather.get(0).description));
                setWeatherIcon(apiResponse.weather.get(0).icon, icon);
            }, error -> {
                Log.e("API", error.toString());
            });

            ((CustomApp) this.getApplication()).queue.add(req);
        });

        Button forecast = findViewById(R.id.button2);
        forecast.setOnClickListener(v -> {
            Intent intend = new Intent(MainActivity.this, Forecast.class);
            intend.putExtra("city", input.getText().toString());
            startActivity(intend);
        });
    }

    private void setWeatherIcon(String iconCode, ImageView imageView) {
        // Build the resource name dynamically
        String resourceName = "icon_" + iconCode;

        // Get the resource ID for the drawable
        int resourceId = imageView.getContext().getResources().getIdentifier(
                resourceName, "drawable", imageView.getContext().getPackageName()
        );

        if (resourceId != 0) {
            // Set the image resource dynamically
            imageView.setImageResource(resourceId);
        }
    }
}