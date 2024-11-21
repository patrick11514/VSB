package cz.patrick115.weatherapp;

public class ListItem {
    public String date;
    public String desc;
    public double temp;
    public double wind;
    public float clouds;
    public String icon;


    public ListItem(String date, String desc, double temp, double wind, float clouds, String icon) {
        this.date = date;
        this.desc = desc;
        this.temp = temp;
        this.wind = wind;
        this.clouds = clouds;
        this.icon = icon;
    }
}