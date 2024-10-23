package com.patrick115.c03;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.ColorStateList;
import android.content.res.Resources;
import android.graphics.Color;
import android.graphics.Shader;
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
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.github.mikephil.charting.charts.BarChart;
import com.github.mikephil.charting.charts.LineChart;
import com.github.mikephil.charting.charts.PieChart;
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
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class MainActivity extends AppCompatActivity {
    Utils.GraphType graphType = Utils.GraphType.Bar;

    double money = 0;
    double interest = 0;

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

        SharedPreferences preferences = getSharedPreferences("settings", Context.MODE_PRIVATE);
        graphType = Utils.deserialize(preferences.getInt("graphType", Utils.serialize(Utils.GraphType.Bar)));

        TextView money = findViewById(R.id.moneyPut);
        TextView interest = findViewById(R.id.interest);
        TextView period = findViewById(R.id.period);
        TextView monthly = findViewById(R.id.monthInvestText);

        SeekBar seekMonthly = findViewById(R.id.monthInvest);

        seekMonthly.setProgress(preferences.getInt("monthly", 1));
        seekMonthly.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                recalculate();
            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {

            }
        });

        SeekBar seekMoney = findViewById(R.id.moneySet);
        seekMoney.setProgress(preferences.getInt("money", 3));
        seekMoney.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
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
        seekInterest.setProgress(preferences.getInt("interest", 3));
        seekInterest.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
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
        seekPeriod.setProgress(preferences.getInt("period", 5));
        seekPeriod.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
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
        changeSoupatkaColor();
    }

    private void recalculate() {
        SeekBar seekMoney = findViewById(R.id.moneySet);
        SeekBar seekInterest = findViewById(R.id.interestBar);
        SeekBar seekPeriod = findViewById(R.id.periodBar);
        SeekBar seekMonthly = findViewById(R.id.monthInvest);

        TextView savedMoney = findViewById(R.id.savedMoney);
        TextView savedInterest = findViewById(R.id.savedDiff);


        /*SOURCE: https://investovaniproholky.cz/kalkulacka-slozeneho-uroceni/
         const investedValue=monthlyInvestment*(investmentYears*12)+startInvestment;
          const rate=annualInterestRate/12;
          const pow=Math.pow(1+rate,investmentYears*12);
          const pmt=-1*monthlyInvestment;const pv=-1*startInvestment;
          const finalValue=(pmt*(1-pow)/rate)-pv*pow;
          const receivedInterest=finalValue-investedValue;
         */

        //Save values to storage
        SharedPreferences preferences = getSharedPreferences("settings", Context.MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        editor.putInt("money", seekMoney.getProgress());
        editor.putInt("interest", seekInterest.getProgress());
        editor.putInt("period", seekPeriod.getProgress());
        editor.putInt("monthly", seekMonthly.getProgress());
        editor.apply();

        money = seekMoney.getProgress() * 10000;
        double inter = (double)seekInterest.getProgress() / 100;
        double period = seekPeriod.getProgress();
        double monthly = seekMonthly.getProgress() * 1000;

        TextView moneyText = findViewById(R.id.moneyPut);
        moneyText.setText(getString(R.string.money_put, (int)money));

        TextView interestText = findViewById(R.id.interest);
        interestText.setText(getString(R.string.interest_put, (int)(inter * 100)));

        TextView periodText = findViewById(R.id.period);
        periodText.setText(getString(R.string.period_put, (int)period));

        TextView monthlyText = findViewById(R.id.monthInvestText);
        monthlyText.setText(getString(R.string.month_put, (int)monthly));

        double investedValue = monthly * (period * 12) + money;
        double monthRate = inter / 12;
        double pow = Math.pow(1+monthRate, period * 12);
        double pmt = -1 * monthly;
        double pv = -1 * money;
        double result = (pmt * (1-pow) / monthRate)-pv*pow;
        interest = result - investedValue;
        savedMoney.setText(getString(R.string.money, (int)result));
        savedInterest.setText(getString(R.string.interest, (int)interest));

        int primary = preferences.getInt("primary", Color.parseColor("#8BE8FE"));
        int secondary = preferences.getInt("secondary", Color.parseColor("#00FEFE"));

        switch(graphType) {
            case Bar:
                Utils.setBarGraph(findViewById(R.id.bar_chart), (float)money, (float)interest, primary, secondary);
                break;
            case Pie:
                Utils.setPieGraph(findViewById(R.id.pie_chart), (float)money, (float)interest, primary, secondary);
                break;
        }
    }

    private void changeSoupatkaColor() {
        SharedPreferences settings = getSharedPreferences("settings", Context.MODE_PRIVATE);
        int primary = settings.getInt("primary", Color.parseColor("#8BE8FE"));
        int secondary = settings.getInt("secondary", Color.parseColor("#00FEFE"));

        ArrayList<SeekBar> seekBars = new ArrayList<>();
        seekBars.add(findViewById(R.id.moneySet));
        seekBars.add(findViewById(R.id.interestBar));
        seekBars.add(findViewById(R.id.periodBar));
        seekBars.add(findViewById(R.id.monthInvest));

        for (SeekBar seekbar : seekBars) {
            seekbar.setProgressTintList(ColorStateList.valueOf(primary));
            seekbar.setThumbTintList(ColorStateList.valueOf(primary));
        }
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == 1) {
            if(resultCode == Activity.RESULT_OK) {
                assert data != null;
                graphType = (Utils.GraphType) data.getSerializableExtra("graphType");
                ResolveGraphDisplay();
                recalculate();

                SharedPreferences preferences = getSharedPreferences("settings", Context.MODE_PRIVATE);
                SharedPreferences.Editor editor = preferences.edit();
                editor.putInt("graphType", Utils.serialize(graphType));
                editor.apply();
            }
        } else if (requestCode == 2) {
            if(resultCode == Activity.RESULT_OK) {
                //redraw graphs
                ResolveGraphDisplay();
                recalculate();
                changeSoupatkaColor();
            }
        }
    }

    private void ResolveGraphDisplay() {
        BarChart barChart = findViewById(R.id.bar_chart);
        PieChart pieChart = findViewById(R.id.pie_chart);
        switch (graphType) {
            case Bar:
                barChart.setVisibility(View.VISIBLE);
                pieChart.setVisibility(View.INVISIBLE);
                break;
            case Pie:
                barChart.setVisibility(View.INVISIBLE);
                pieChart.setVisibility(View.VISIBLE);
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
            intend.putExtra("money", money);
            intend.putExtra("interest", interest);
            startActivityForResult(intend, 1);
            return true;
        } else if (myId == R.id.menu_save) {
            SharedPreferences preferences = getSharedPreferences("history", Context.MODE_PRIVATE);
            int lastIndex = preferences.getInt("length", 0);
            SharedPreferences.Editor editor = preferences.edit();
            editor.putString("date_" + lastIndex,  new SimpleDateFormat("yyyy-MM-dd HH:mm:ss").format(new Date()));
            editor.putFloat("money_" + lastIndex, (float)money);
            editor.putFloat("interest_" + lastIndex, (float)interest);
            editor.putInt("length", lastIndex + 1);
            editor.apply();

            Toast.makeText(this, "Uloženo", Toast.LENGTH_SHORT).show();
        } else if (myId == R.id.menu_history) {
            Intent intend = new Intent(MainActivity.this, History.class);
            startActivity(intend);
        } else if (myId == R.id.menu_settings) {
            Intent intend = new Intent(MainActivity.this, Settings.class);
            startActivityForResult(intend, 2);
        }

        return super.onOptionsItemSelected(item);
    }
}