package com.patrick115.c03;

import android.graphics.Color;
import android.os.Bundle;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_main);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        LineChart chart = findViewById(R.id.chart);
        chart.setBackgroundColor(Color.LTGRAY);

        List<Entry> entries = new ArrayList<>();
        entries.add(new Entry(0,0));
        entries.add(new Entry(5,5));
        entries.add(new Entry(10,5));
        entries.add(new Entry(15,15));
        entries.add(new Entry(20,20));


        LineDataSet dataSet = new LineDataSet(entries, "Label");

        dataSet.setLineWidth(20);

        LineData lineData   = new LineData(dataSet);
        chart.setData(lineData);
        chart.invalidate();
    }
}