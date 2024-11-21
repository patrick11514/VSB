package cz.patrick115.weatherapp;

import com.google.gson.annotations.SerializedName;

import java.util.List;

public class ApiResponse {
    @SerializedName("coord")
    public Coord coord;
    @SerializedName("weather")
    public List<Weather> weather;
    @SerializedName("base")
    public String base;
    @SerializedName("main")
    public Main main;
    @SerializedName("visibility")
    public int visibility;
    @SerializedName("wind")
    public Wind wind;
    @SerializedName("rain")
    public Rain rain;
    @SerializedName("clouds")
    public Clouds clouds;
    @SerializedName("dt")
    public long dt;
    @SerializedName("sys")
    public Sys sys;
    @SerializedName("timezone")
    public int timezone;
    @SerializedName("id")
    public int id;
    @SerializedName("name")
    public String name;
    @SerializedName("cod")
    public int cod;
    public static class Coord {
        @SerializedName("lon")
        public double lon;
        @SerializedName("lat")
        public double lat;
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
        @SerializedName("humidity")
        public int humidity;
        @SerializedName("sea_level")
        public int seaLevel;
        @SerializedName("grnd_level")
        public int grndLevel;
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

    public static class Clouds {
        @SerializedName("all")
        public int all;
    }
    public static class Sys {
        @SerializedName("type")
        public int type;
        @SerializedName("id")
        public int id;
        @SerializedName("country")
        public String country;
        @SerializedName("sunrise")
        public long sunrise;
        @SerializedName("sunset")
        public long sunset;
    }
}