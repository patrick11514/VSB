package cz.patrick115.weatherapp;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import androidx.fragment.app.Fragment;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;

import com.android.volley.toolbox.Volley;
import com.google.android.material.bottomnavigation.BottomNavigationView;

public class MainActivity extends AppCompatActivity {
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);

        CustomApp application = (CustomApp) getApplication();

        application.queue = Volley.newRequestQueue(this);

        SharedPreferences sharedPref = getPreferences(Context.MODE_PRIVATE);
        String city = sharedPref.getString("city", null);
        if (city != null) {
            application.city = city;
        }

        BottomNavigationView bottomNav = findViewById(R.id.bottom_navigation);
        bottomNav.setOnNavigationItemSelectedListener( item -> {
            // By using switch we can easily get
            // the selected fragment
            // by using there id.
            Fragment selectedFragment = null;
            int itemId = item.getItemId();
            if (itemId == R.id.weather) {
                selectedFragment = new WeatherSelect();
            } else if (itemId == R.id.forecast) {
                selectedFragment = new Forecast();
            } else if (itemId == R.id.graph) {
                selectedFragment = new Graph();
            }
            // It will help to replace the
            // one fragment to other.
            if (selectedFragment != null) {
                getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, selectedFragment).commit();
            }

            return true;
        });

        // as soon as the application opens the first
        // fragment should be shown to the user
        // in this case it is algorithm fragment
        getSupportFragmentManager().beginTransaction().replace(R.id.fragment_container, new WeatherSelect()).commit();

    }
}