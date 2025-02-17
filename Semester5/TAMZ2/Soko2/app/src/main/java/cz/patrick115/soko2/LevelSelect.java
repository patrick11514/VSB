package cz.patrick115.soko2;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.os.Bundle;
import android.util.Log;

import androidx.activity.EdgeToEdge;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;
import androidx.recyclerview.widget.LinearLayoutManager;
import androidx.recyclerview.widget.RecyclerView;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;

public class LevelSelect extends AppCompatActivity {
    private RecyclerView recyclerView;
    ListItemAdapter adapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        EdgeToEdge.enable(this);
        setContentView(R.layout.activity_level_select);
        ViewCompat.setOnApplyWindowInsetsListener(findViewById(R.id.main), (v, insets) -> {
            Insets systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars());
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom);
            return insets;
        });

        DatabaseManager db = new DatabaseManager(this);

        recyclerView = findViewById(R.id.levelList);
        recyclerView.setLayoutManager(new LinearLayoutManager(this));

        // Initialize the data
        List<ListItem> itemList = new ArrayList<>();
         adapter = new ListItemAdapter(itemList);
        recyclerView.setAdapter(adapter);

        List<String> levelsDef = readLevelsFile();
        if (!levelsDef.isEmpty()) {
            List<Level> levels = Level.getLevels(levelsDef);
            ((CustomApplication) this.getApplication()).levels = levels;
            for (Level level: levels) {
                level.assignDB(db);
                itemList.add(new ListItem(level));
            }

            adapter.notifyDataSetChanged();
        }
    }

    private List<String> readLevelsFile() {
        Resources res = getResources();
        InputStream inputStream = res.openRawResource(R.raw.levels);
        BufferedReader reader = new BufferedReader(new InputStreamReader(inputStream));

        List<String> lines = new ArrayList<>();

        try {
            String line;
            while ((line = reader.readLine()) != null) {
                lines.add(line);
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            try {
                reader.close();
                inputStream.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        return lines;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        Log.i("STATE", String.valueOf(resultCode));
        Log.i("REQ", String.valueOf(requestCode));

        if (requestCode == 1) {
            if (resultCode == Activity.RESULT_OK) {
                assert data != null;
                int id = data.getIntExtra("id", 0);
                int next = data.getIntExtra("load", -1);
                adapter.notifyItemChanged(id - 1);

                if (next > -1) {
                    Intent intend = new Intent(LevelSelect.this, CurrentLevel.class);
                    intend.putExtra("id", next);
                    startActivityForResult(intend, 1);
                }
            }
        }
    }
}