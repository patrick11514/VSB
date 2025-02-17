package cz.patrick115.weatherapp;

import android.app.Application;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.TextView;

import com.android.volley.Request;
import com.android.volley.Response;
import com.android.volley.toolbox.StringRequest;
import com.android.volley.toolbox.Volley;
import com.google.android.material.textfield.TextInputEditText;
import com.google.gson.Gson;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link WeatherSelect#newInstance} factory method to
 * create an instance of this fragment.
 */
public class WeatherSelect extends Fragment {

    public WeatherSelect() {
        // Required empty public constructor
    }

    public static WeatherSelect newInstance() {
        WeatherSelect fragment = new WeatherSelect();
        Bundle args = new Bundle();
        fragment.setArguments(args);
        return fragment;
    }

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    public View onCreateView(LayoutInflater inflater, ViewGroup container,
                             Bundle savedInstanceState) {
        
        
        // Inflate the layout for this fragment
        View view = inflater.inflate(R.layout.fragment_weather_select, container, false);

        getActivity().setTitle("Přehled počasí");

        CustomApp application = (CustomApp)getActivity().getApplication();

        TextView temp = view.findViewById(R.id.temp);
        TextView desc = view.findViewById(R.id.desc);
        ImageView icon = view.findViewById(R.id.icon);

        TextInputEditText input = view.findViewById(R.id.town);

        Button search = view.findViewById(R.id.button);
        search.setOnClickListener(v -> {
            //save location
            application.city = input.getText().toString();

            SharedPreferences sharedPref = getActivity().getPreferences(Context.MODE_PRIVATE);
            SharedPreferences.Editor editor = sharedPref.edit();
            editor.putString("city", application.city);
            editor.apply();

            String URL = "https://api.openweathermap.org/data/2.5/weather?q=" + application.city + ",CZ&appid=" + application.API_KEY + "&lang=cz&units=metric";

            StringRequest req = new StringRequest(Request.Method.GET, URL, response -> {
                Gson gson = new Gson();
                application.response = gson.fromJson(response, ApiResponse.class);
                temp.setText(application.response.main.temp + "°C");
                desc.setText(String.valueOf(application.response.weather.get(0).description));
                setWeatherIcon(application.response.weather.get(0).icon, icon);
            }, error -> {
                Log.e("API", error.toString());
            });

            application.queue.add(req);

            URL = "https://api.openweathermap.org/data/2.5/forecast?q=" + application.city + ",CZ&appid=" + application.API_KEY + "&lang=cz&units=metric";

            StringRequest req2 = new StringRequest(Request.Method.GET, URL, response -> {
                Gson gson = new Gson();
                application.forecast = gson.fromJson(response, ForecastApiResponse.class);

                application.forecastItems.clear();

                for (ForecastApiResponse.WeatherData item : application.forecast.weatherDataList) {
                    application.forecastItems.add(new ListItem(
                            item.dtTxt,
                            item.weather.get(0).description,
                            item.main.temp,
                            item.wind.speed,
                            item.clouds.all,
                            item.weather.get(0).icon));
                }

                application.adapter.notifyDataSetChanged();
            }, error -> {
                Log.e("API", error.toString());
            });

            application.queue.add(req2);
        });

        if (application.city != null) {
            input.setText(application.city);

            if (application.response != null) {
                temp.setText(application.response.main.temp + "°C");
                desc.setText(String.valueOf(application.response.weather.get(0).description));
                setWeatherIcon(application.response.weather.get(0).icon, icon);
            } else {
                search.callOnClick();
            }
        }
        
        return view;
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