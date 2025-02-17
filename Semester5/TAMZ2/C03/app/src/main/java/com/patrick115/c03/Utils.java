package com.patrick115.c03;

import android.graphics.Color;

import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.charts.PieChart;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.data.PieData;
import com.github.mikephil.charting.data.PieDataSet;
import com.github.mikephil.charting.data.PieEntry;

import java.util.ArrayList;

public class Utils {
    public enum GraphType {
        Bar,
        Pie
    };

    public static int serialize(GraphType graphType) {
        switch (graphType) {
            case Bar: return 0;
            case Pie: return 1;
        }
        return -1;
    }

    public static GraphType deserialize(int number) {
        switch (number) {
            case 0: return GraphType.Bar;
            case 1: return GraphType.Pie;
        }

        return GraphType.Bar;
    }

    public static void setBarGraph(BarChart chart, float money, float interest, int primary, int secondary) {
        chart.getLegend().setTextColor(Color.WHITE);
        chart.setPinchZoom(false);
        chart.getDescription().setEnabled(false);

        float graphMin = Math.max(0, Math.min(money, interest) - 10000);

        XAxis xAxis = chart.getXAxis();
        xAxis.setCenterAxisLabels(true);
        xAxis.setDrawGridLines(false);
        xAxis.setDrawLabels(false);
        xAxis.setTextColor(Color.WHITE);
        YAxis yAxis = chart.getAxisLeft();
        yAxis.setAxisMinimum(graphMin);
        yAxis.setDrawGridLines(false);
        yAxis.setTextColor(Color.WHITE);
        YAxis yAxis2 = chart.getAxisRight();
        yAxis2.setAxisMinimum(graphMin);
        yAxis2.setDrawGridLines(false);
        yAxis2.setTextColor(Color.WHITE);

        ArrayList<BarEntry> entries = new ArrayList<>();
        entries.add(new BarEntry(0, (float)money));

        ArrayList<BarEntry> entries2 = new ArrayList<>();
        entries2.add(new BarEntry(1, (float) interest));

        BarDataSet data = new BarDataSet(entries, "Vklad");
        data.setValueTextColor(Color.WHITE);
        BarDataSet data2 = new BarDataSet(entries2, "Úroky");
        data2.setValueTextColor(Color.WHITE);
        data.setColor(primary);
        data2.setColor(secondary);
        chart.setData(new BarData(data, data2));
        chart.invalidate();
    }

    public static void setPieGraph(PieChart chart, float money, float interest, int primary, int secondary) {
        chart.getLegend().setTextColor(Color.WHITE);
        chart.getDescription().setEnabled(false);
        chart.setRotationEnabled(false);

        ArrayList<PieEntry> entries = new ArrayList<>();
        entries.add(new PieEntry((float)money, "Vklad"));
        entries.add(new PieEntry((float)interest, "Úroky"));
        PieDataSet dataSet = new PieDataSet(entries, "");

        ArrayList<Integer> colors = new ArrayList<>();
        colors.add(primary);//Color.parseColor("#00FEFE"));
        colors.add(secondary);//Color.parseColor("#8BE8FE"));
        dataSet.setColors(colors);
        dataSet.setValueTextSize(12f);
        PieData data = new PieData(dataSet);
        chart.setData(data);
        chart.setHoleRadius(40);
        chart.setEntryLabelColor(Color.BLACK);
        chart.invalidate();
    }
}
