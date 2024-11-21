package cz.patrick115.weatherapp;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.toolbox.StringRequest;
import com.google.gson.Gson;

import java.util.ArrayList;
import java.util.List;

public class Forecast extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_forecast);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        RecyclerView recyclerView = findViewById(R.id.forecast);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));

        // Initialize the data
        List<ListItem> itemList = new ArrayList<>();
        ListItemAdapter adapter = new ListItemAdapter(itemList);
        recyclerView.setAdapter(adapter);

        Intent intent = getIntent();
        String city = intent.getStringExtra("city");

        String URL = "https://api.openweathermap.org/data/2.5/forecast?q=" + city + ",CZ&appid=" + ((CustomApp) this.getApplication()).API_KEY + "&lang=cz&units=metric";

        StringRequest req = new StringRequest(Request.Method.GET, URL, (Response.Listener<String>) response -> {
            Gson gson = new Gson();
            ForecastApiResponse apiResponse = gson.fromJson(response, ForecastApiResponse.class);

            for (ForecastApiResponse.WeatherData item : apiResponse.weatherDataList) {
                itemList.add(new ListItem(
                        item.dtTxt,
                        item.weather.get(0).description,
                        item.main.temp,
                        item.wind.speed,
                        item.clouds.all,
                        item.weather.get(0).icon));
            }

            adapter.notifyDataSetChanged();
        }, error -> {
            Log.e("API", error.toString());
        });

        ((CustomApp) this.getApplication()).queue.add(req);
    }
}