package com.patrick115.c03;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.cardview.widget.CardView;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

public class History extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_history);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        LinearLayout layout = findViewById(R.id.historyLayout);

        SharedPreferences preferences = getSharedPreferences("history", Context.MODE_PRIVATE);
        int end = preferences.getInt("length", 0);

        for (int i = end - 1; i >= 0; --i) {
            float money = preferences.getFloat("money_" + i, 0);
            float interest = preferences.getFloat("interest_" + i, 0);
            String date = preferences.getString("date_" + i, "Unknown date");

            CardView card = new CardView(this);
            card.setPadding(dpToPix(40), dpToPix(40), dpToPix(40), dpToPix(40));
            card.setScrollContainer(true);
            card.setLayoutParams(new LinearLayout.LayoutParams(
                    LinearLayout.LayoutParams.MATCH_PARENT,  // width: match_parent
                    LinearLayout.LayoutParams.WRAP_CONTENT   // height: wrap_content
            ));
            card.setRadius(dpToPix(20));
            card.setElevation(dpToPix(5));
            card.setPreventCornerOverlap(true);
            card.setUseCompatPadding(true);

            LinearLayout newLayout = new LinearLayout(this);
            newLayout.setPadding(dpToPix(10), dpToPix(10), dpToPix(10), dpToPix(10));
            newLayout.setOrientation(LinearLayout.VERTICAL);

            card.addView(newLayout);

            TextView dateText = new TextView(this);
            dateText.setText(date);
            newLayout.addView(dateText);

            TextView moneyText = new TextView(this);
            moneyText.setText(getString(R.string.money, (int)money));
            moneyText.setTextSize(20);
            newLayout.addView(moneyText);

            TextView interestText = new TextView(this);
            interestText.setText(getString(R.string.interest, (int)interest));
            interestText.setTextSize(20);
            newLayout.addView(interestText);

            layout.addView(card);
        }
    }


    public int dpToPix(int dp) {
        // Get the screen's density scale (pixels per dp)
        float density = getResources().getDisplayMetrics().density;
        // Convert the dp value to pixels based on density
        return Math.round(dp * density);
    }
}