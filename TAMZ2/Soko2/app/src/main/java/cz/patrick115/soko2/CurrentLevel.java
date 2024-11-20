package cz.patrick115.soko2;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import androidx.activity.EdgeToEdge;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import org.w3c.dom.Text;

import java.util.List;

public class CurrentLevel extends AppCompatActivity implements ViewEventHandler {

    Level currentLevel;
    SokoView sokoView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_current_level);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        sokoView = findViewById(R.id.sokoView);
        sokoView.addEventHandler(this);

        Intent intent = this.getIntent();
        int id = intent.getIntExtra("id", 0);

        currentLevel = ((CustomApplication) this.getApplication()).levels.get(id - 1);

        sokoView.bind(currentLevel);
    }

    @Override
    public void onUpdate(int data) {
        TextView textView = findViewById(R.id.score);
        textView.setText("Score: " + data);
    }

    @Override
    public void onUpdate() {
        //change to next level
        /*currentLevel.save();
        currentLevel = ((CustomApplication) this.getApplication()).levels.get(currentLevel.getId());
        sokoView.bind(currentLevel);*/
        currentLevel.save();
        Intent returnIntent = new Intent();
        returnIntent.putExtra("id", currentLevel.getId());
        returnIntent.putExtra("load", currentLevel.getId() + 1);
        setResult(Activity.RESULT_OK, returnIntent);
        finish();
    }


    protected void onPause() {
        super.onPause();
        currentLevel.save();
    }

    public void onBackPressed() {
        Intent returnIntent = new Intent();
        returnIntent.putExtra("id", currentLevel.getId());
        Log.i("ID:" , String.valueOf(currentLevel.getId()));
        setResult(Activity.RESULT_OK, returnIntent);

        super.onBackPressed();
    }
}