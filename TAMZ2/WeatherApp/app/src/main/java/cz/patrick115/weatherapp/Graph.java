package cz.patrick115.weatherapp;

import android.graphics.Color;
import android.os.Bundle;

import androidx.fragment.app.Fragment;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.util.ArrayList;
import java.util.List;

/**
 * A simple {@link Fragment} subclass.
 * Use the {@link Graph#newInstance} factory method to
 * create an instance of this fragment.
 */
public class Graph extends Fragment {

    private List<Entry> getTemperatureEntries(List<ForecastApiResponse.WeatherData> weatherDataList) {
        List<Entry> entries = new ArrayList<>();
        for (int i = 0; i < weatherDataList.size(); i++) {
            double temp = weatherDataList.get(i).main.temp;
            entries.add(new Entry(i, (float) temp));
        }
        return entries;
    }

    private List<Entry> getWindSpeedEntries(List<ForecastApiResponse.WeatherData> weatherDataList) {
        List<Entry> entries = new ArrayList<>();
        for (int i = 0; i < weatherDataList.size(); i++) {
            double speed = weatherDataList.get(i).wind.speed;
            entries.add(new Entry(i, (float) speed));
        }
        return entries;
    }

    private List<Entry> getCloudinessEntries(List<ForecastApiResponse.WeatherData> weatherDataList) {
        List<Entry> entries = new ArrayList<>();
        for (int i = 0; i < weatherDataList.size(); i++) {
            int cloudiness = weatherDataList.get(i).clouds.all;
            entries.add(new Entry(i, cloudiness));
        }
        return entries;
    }

    private List<String> getTimeEntries(List<ForecastApiResponse.WeatherData> weatherDataList) {
        List<String> entries = new ArrayList<>();
        for (int i = 0; i < weatherDataList.size(); i++) {
            String text = weatherDataList.get(i).dtTxt;
            String[] splited = text.split(" ");
            String[] date = splited[0].split("-");
            String time = date[2] + "." + date[1] + " " + splited[1]; // date + time
            entries.add(time);
        }
        return entries;
    }

    private LineDataSet createLineDataSet(List<Entry> entries, String label, int color) {
        LineDataSet dataSet = new LineDataSet(entries, label);
        dataSet.setColor(color);
        dataSet.setLineWidth(2f);
        dataSet.setValueTextColor(color);
        dataSet.setCircleColor(color);
        dataSet.setCircleRadius(3f);
        dataSet.setDrawValues(false);
        return dataSet;
    }

    private LineData getChartData(List<ForecastApiResponse.WeatherData> weatherDataList) {
        List<Entry> temperatureEntries = getTemperatureEntries(weatherDataList);
        List<Entry> windSpeedEntries = getWindSpeedEntries(weatherDataList);
        List<Entry> cloudinessEntries = getCloudinessEntries(weatherDataList);

        LineDataSet temperatureDataSet = createLineDataSet(temperatureEntries, "Temperature", Color.RED);
        LineDataSet windSpeedDataSet = createLineDataSet(windSpeedEntries, "Wind Speed", Color.BLUE);
        LineDataSet cloudinessDataSet = createLineDataSet(cloudinessEntries, "Cloudiness", Color.GREEN);

        return new LineData(temperatureDataSet, windSpeedDataSet, cloudinessDataSet);
    }

    private void setupChart(LineChart lineChart, LineData lineData) {
        lineChart.setData(lineData);
        lineChart.getDescription().setEnabled(false);

        lineChart.getAxisRight().setEnabled(false);
        lineChart.getAxisLeft().setTextColor(Color.WHITE);

        XAxis xAxis = lineChart.getXAxis();
        xAxis.setDrawGridLines(false);
        xAxis.setTextColor(Color.WHITE);

        lineChart.animateX(1000);
        lineChart.getLegend().setTextColor(Color.WHITE);
        lineChart.invalidate(); // Refresh the chart
    }

    public Graph() {
        // Required empty public constructor
    }

    /**
     * Use this factory method to create a new instance of
     * this fragment using the provided parameters.
     *
     * @param param1 Parameter 1.
     * @param param2 Parameter 2.
     * @return A new instance of fragment Graph.
     */
    // TODO: Rename and change types and number of parameters
    public static Graph newInstance(String param1, String param2) {
        Graph fragment = new Graph();
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
        View view = inflater.inflate(R.layout.fragment_graph, container, false);

        CustomApp application = (CustomApp) getActivity().getApplication();

        getActivity().setTitle("Graf předpovědi");

        LineChart chart = view.findViewById(R.id.lineChart);

        List<ForecastApiResponse.WeatherData> weatherDataList = application.forecast.weatherDataList;

        XAxis xAxis = chart.getXAxis();
        xAxis.setValueFormatter(new IdkFormatter(getTimeEntries(weatherDataList)));
        xAxis.setGranularity(1f);

        LineData lineData = getChartData(weatherDataList);
        setupChart(chart, lineData);

        return view;
    }
}