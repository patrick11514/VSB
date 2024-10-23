package com.patrick115.c03;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.charts.PieChart;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.PieData;
import com.github.mikephil.charting.data.PieDataSet;
import com.github.mikephil.charting.data.PieEntry;
import com.github.mikephil.charting.highlight.Highlight;
import com.github.mikephil.charting.listener.OnChartValueSelectedListener;

import java.util.ArrayList;

public class GraphTypeSelector extends AppCompatActivity {
    double money = 0;
    double interest = 0;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_graph_type_selector);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        Intent me = getIntent();
        money = me.getDoubleExtra("money", 0);
        interest = me.getDoubleExtra("interest", 0);

        SharedPreferences settings = getSharedPreferences("settings", Context.MODE_PRIVATE);

        int primary = settings.getInt("primary", Color.parseColor("#8BE8FE"));
        int secondary = settings.getInt("secondary", Color.parseColor("#00FEFE"));

        Utils.setBarGraph(findViewById(R.id.bar_chart), (float)money, (float)interest, primary, secondary);
        Utils.setPieGraph(findViewById(R.id.pie_chart), (float)money, (float)interest, primary, secondary);

        CardView barView = findViewById(R.id.bar_view);
        barView.setOnClickListener(v -> finishActivity(Utils.GraphType.Bar));
        BarChart barChart = findViewById(R.id.bar_chart);
        barChart.setOnClickListener(v -> finishActivity(Utils.GraphType.Bar));


        CardView pieView = findViewById(R.id.pie_view);
        pieView.setOnClickListener(v -> finishActivity(Utils.GraphType.Pie));
        PieChart pieChart = findViewById(R.id.pie_chart);
        pieChart.setOnChartValueSelectedListener(new OnChartValueSelectedListener() {
            @Override
            public void onValueSelected(Entry e, Highlight h) {
                finishActivity(Utils.GraphType.Pie);
            }

            @Override
            public void onNothingSelected() {
                finishActivity(Utils.GraphType.Pie);
            }
        });
    }

    private void finishActivity(Utils.GraphType type) {
        Intent returnIntent = new Intent();
        returnIntent.putExtra("graphType", type);
        setResult(Activity.RESULT_OK,returnIntent);
        finish();
    }
}