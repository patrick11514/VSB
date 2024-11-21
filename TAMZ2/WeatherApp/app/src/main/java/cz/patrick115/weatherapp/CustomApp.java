package cz.patrick115.weatherapp;

import android.app.Application;

import com.android.volley.RequestQueue;

public class CustomApp extends Application {
    RequestQueue queue;
    final String API_KEY = "e9d4be604764e03880645ab3f9ccd060";
}
