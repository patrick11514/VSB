package com.patrick115.c03;

import android.content.Intent;
import android.content.res.Resources;
import android.graphics.Color;
import android.os.Bundle;
import android.util.Log;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.components.Description;
import com.github.mikephil.charting.components.XAxis;
import com.github.mikephil.charting.components.YAxis;
import com.github.mikephil.charting.data.BarData;
import com.github.mikephil.charting.data.BarDataSet;
import com.github.mikephil.charting.data.BarEntry;
import com.github.mikephil.charting.data.Entry;
import com.github.mikephil.charting.data.LineData;
import com.github.mikephil.charting.data.LineDataSet;

import java.sql.Array;
import java.util.ArrayList;
import java.util.List;

public class MainActivity extends AppCompatActivity {

    enum GraphType {
        Bar,
        Pie
    };

    GraphType graphType = GraphType.Bar;

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


        BarChart chart = findViewById(R.id.bar_chart);
        chart.setBackgroundColor(getColor(R.color.white));
        chart.setPinchZoom(false);
        chart.setDrawGridBackground(false);

        Description desc = new Description();
        desc.setText("");
        chart.setDescription(desc);


        TextView money = findViewById(R.id.moneyPut);
        TextView interest = findViewById(R.id.interest);
        TextView period = findViewById(R.id.period);

        SeekBar seekMoney = findViewById(R.id.moneySet);
        seekMoney.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                money.setText(getString(R.string.money_put, progress * 10000));
                recalculate();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }

        });

        SeekBar seekInterest = findViewById(R.id.interestBar);
        seekInterest.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                interest.setText(getString(R.string.interest_put, progress));
                recalculate();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        SeekBar seekPeriod = findViewById(R.id.periodBar);
        seekPeriod.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                period.setText(getString(R.string.period_put, progress));
                recalculate();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        recalculate();
        ResolveGraphDisplay();
    }

    private void recalculate() {
        SeekBar seekMoney = findViewById(R.id.moneySet);
        SeekBar seekInterest = findViewById(R.id.interestBar);
        SeekBar seekPeriod = findViewById(R.id.periodBar);

        TextView savedMoney = findViewById(R.id.savedMoney);
        TextView savedInterest = findViewById(R.id.savedDiff);

        int money = seekMoney.getProgress() * 10000;

        double result = money * Math.pow(1 + (float)seekInterest.getProgress() / 100, seekPeriod.getProgress());
        double interest = result - money;
        savedMoney.setText(getString(R.string.money, (int)result));
        savedInterest.setText(getString(R.string.interest, (int)interest));

        BarChart chart = findViewById(R.id.bar_chart);

        float graphMin = Math.max(0, (float)Math.min(money, interest) - 10000);

        XAxis xAxis = chart.getXAxis();
        xAxis.setCenterAxisLabels(true);
        YAxis yAxis = chart.getAxisLeft();
        yAxis.setAxisMinimum(graphMin);
        YAxis yAxis2 = chart.getAxisRight();
        yAxis2.setAxisMinimum(graphMin);

        ArrayList<BarEntry> entries = new ArrayList<>();
        entries.add(new BarEntry(0, money));

        ArrayList<BarEntry> entries2 = new ArrayList<>();
        entries2.add(new BarEntry(1, (float) interest));

        BarDataSet data = new BarDataSet(entries, "Vklad");
        BarDataSet data2 = new BarDataSet(entries2, "Úroky");
        data.setColor(Color.CYAN);
        chart.setData(new BarData(data, data2));
        chart.invalidate();
    }

    private void ResolveGraphDisplay() {
        BarChart barChart = findViewById(R.id.bar_chart);
        switch (graphType) {
            case Bar:
                barChart.setVisibility(View.VISIBLE);
                break;
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(@NonNull MenuItem item) {
        int myId = item.getItemId();

        if (myId == R.id.menu_graph_type) {
            Intent intend = new Intent(MainActivity.this, GraphTypeSelector.class);
            startActivity(intend);
            return true;
        }

        return super.onOptionsItemSelected(item);
    }
}