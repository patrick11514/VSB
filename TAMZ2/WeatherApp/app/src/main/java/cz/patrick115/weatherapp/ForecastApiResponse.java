package cz.patrick115.weatherapp;

import com.google.gson.annotations.SerializedName;

import java.util.List;

public class ForecastApiResponse {
        @SerializedName("cod")
        public String cod;

        @SerializedName("message")
        public int message;

        @SerializedName("cnt")
        public int cnt;

        @SerializedName("list")
        public List<WeatherData> weatherDataList;

        @SerializedName("city")
        public City city;

        public static class WeatherData {

            @SerializedName("dt")
            public long dt;

            @SerializedName("main")
            public Main main;

            @SerializedName("weather")
            public List<Weather> weather;

            @SerializedName("clouds")
            public Clouds clouds;

            @SerializedName("wind")
            public Wind wind;

            @SerializedName("visibility")
            public int visibility;

            @SerializedName("pop")
            public double pop;

            @SerializedName("rain")
            public Rain rain;

            @SerializedName("sys")
            public Sys sys;

            @SerializedName("dt_txt")
            public String dtTxt;
        }

        public static class Main {
            @SerializedName("temp")
            public double temp;

            @SerializedName("feels_like")
            public double feelsLike;

            @SerializedName("temp_min")
            public double tempMin;

            @SerializedName("temp_max")
            public double tempMax;

            @SerializedName("pressure")
            public int pressure;

            @SerializedName("sea_level")
            public int seaLevel;

            @SerializedName("grnd_level")
            public int grndLevel;

            @SerializedName("humidity")
            public int humidity;

            @SerializedName("temp_kf")
            public double tempKf;
        }

        public static class Weather {
            @SerializedName("id")
            public int id;

            @SerializedName("main")
            public String main;

            @SerializedName("description")
            public String description;

            @SerializedName("icon")
            public String icon;
        }

        public static class Clouds {
            @SerializedName("all")
            public int all;
        }

        public static class Wind {
            @SerializedName("speed")
            public double speed;

            @SerializedName("deg")
            public int deg;

            @SerializedName("gust")
            public double gust;
        }

        public static class Rain {
            @SerializedName("1h")
            public double oneHour;
        }

        public static class Sys {
            @SerializedName("pod")
            public String pod;
        }

        public static class City {
            @SerializedName("id")
            public int id;

            @SerializedName("name")
            public String name;

            @SerializedName("coord")
            public Coord coord;

            @SerializedName("country")
            public String country;

            @SerializedName("population")
            public int population;

            @SerializedName("timezone")
            public int timezone;

            @SerializedName("sunrise")
            public long sunrise;

            @SerializedName("sunset")
            public long sunset;
        }

        public static class Coord {
            @SerializedName("lat")
            public double lat;

            @SerializedName("lon")
            public double lon;
        }
    }
