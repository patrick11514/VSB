package cz.patrick115.weatherapp;

import android.app.Application;

import com.android.volley.RequestQueue;

import java.util.ArrayList;
import java.util.List;

public class CustomApp extends Application {
    String city = null;
    RequestQueue queue;
    final String API_KEY = "e9d4be604764e03880645ab3f9ccd060";
    ApiResponse response = null;
    ForecastApiResponse forecast = null;
    List<ListItem> forecastItems = new ArrayList<>();
    ListItemAdapter adapter = new ListItemAdapter(this.forecastItems);
}
