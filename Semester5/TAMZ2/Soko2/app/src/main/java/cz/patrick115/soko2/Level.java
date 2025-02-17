package cz.patrick115.soko2;


import android.database.Cursor;
import android.util.Log;

import java.util.ArrayList;
import java.util.List;

import kotlin.Pair;
import kotlin.Triple;

enum Block {
    Floor,
    Wall,
    Box,
    X,
    Player,
    BoxOnX,
    PlayerOnX
};

public class Level {
    static List<Level> getLevels(List<String> lines) {
        List<Level> levels = new ArrayList<>();

        int lineCount = 0;
        while (lineCount < lines.size()) {
            String line = lines.get(lineCount++);
            String[] splited = line.split(" ");
            int width = Integer.parseInt(splited[0]);
            int height = Integer.parseInt(splited[1]);

            List<String> levelLines = new ArrayList<>();

            for (int i = 0; i < height; ++i) {
                levelLines.add(lines.get(lineCount++));
            }

            levels.add(new Level(width, height, levelLines));
        }

        return levels;
    }

    static int id = 0;
    private int myId;
    private int width;
    private int height;

    private int[] level;
    private int[] currentState;
    private DatabaseManager db;
    int score = 0;
    private ViewEventHandler eventHandler;

    public Level(int width, int height, List<String> lines) {
        this.myId = ++id;
        this.level = new int[width * height];
        this.currentState = new int[width * height];
        this.width = width;
        this.height = height;

        for (int l = 0; l < height; ++l) {
            String line = lines.get(l);
            for (int i = 0; i < width; ++i) {
                char c = line.charAt(i);
                this.level[l * width + i] = c - '0';
            }
        }

        this.reset();
    }

    void reset() {
        System.arraycopy(level, 0, currentState, 0, level.length);
    }

    int at(int x, int y) {
        return this.currentState[y * this.width + x];
    }
    void set(int x, int y, Block block) {
        this.currentState[y * this.width + x] = block.ordinal();
    }
    void assignDB(DatabaseManager db) {
        this.db = db;

        //try to load backup
        Cursor cur = this.db.select(this.myId);
        Log.i("COUNT", String.valueOf(cur.getCount()));
        if (cur != null) {
            while (cur.moveToNext()) {
                int width = cur.getInt(cur.getColumnIndexOrThrow(DatabaseHelper.COLUMN_WIDTH));
                int height = cur.getInt(cur.getColumnIndexOrThrow(DatabaseHelper.COLUMN_HEIGHT));

                if (width != this.width || height != this.height) {
                    cur.close();
                    return; // don't load
                }

                int score = cur.getInt(cur.getColumnIndexOrThrow(DatabaseHelper.COLUMN_SCORE));
                String data = cur.getString(cur.getColumnIndexOrThrow(DatabaseHelper.COLUMN_DATA));

                int[] level = new int[width * height];
                for (int i = 0; i < data.length(); ++i) {
                    level[i] = data.charAt(i) - '0';
                    if (level[i] == Block.Wall.ordinal() && level[i] != this.currentState[i]) {
                        cur.close();
                        return; // don't load invalid walls
                    }
                }

                //level is correct
                System.arraycopy(level, 0, currentState, 0, level.length);
                this.score = score;

                if(this.eventHandler != null) {
                    this.eventHandler.onUpdate();
                }

                cur.close();
                return;
            }
        }

    }

    private String serialize() {
        StringBuilder sb = new StringBuilder();
        for (int i = 0; i < this.width * this.height; ++i) {
            int state = this.currentState[i];
            if (state == Block.Player.ordinal() && (this.level[i] == Block.X.ordinal() || this.level[i] == Block.PlayerOnX.ordinal())) {
                Log.i("IDK", "YAY");
                state = Block.PlayerOnX.ordinal();
            }

            sb.append(state);
        }
        return sb.toString();
    }

    void save() {
        if (!this.db.exists(this.myId)) {
            this.db.insertData(this.myId, this.width, this.height, this.score, this.serialize());
            return;
        }

        this.db.update(this.myId, this.width, this.height, this.score, this.serialize());
    }

    Triple<Integer, Integer, Boolean> getPlayer() {
        for (int l = 0; l < this.height; ++l) {
            for (int i = 0; i < this.width; ++i) {
                int cur = this.currentState[l * this.width + i];
                if (cur == Block.PlayerOnX.ordinal()) {
                    set(i, l, Block.Player);
                    return new Triple<>(i, l, true);
                }
                if (cur == Block.Player.ordinal()) {
                    return new Triple<>(i, l, false);
                }
            }
        }

        //
        return new Triple<>(0, 0, false);
    }

    Pair<Integer, Integer> getSize() {
        return new Pair<>(this.width, this.height);
    }

    boolean win() {
        for (int blockId : currentState) {
            //some boxes are still there
            if (Block.values()[blockId] == Block.Box) return false;
        }

        return true;
    }

    int getScore() {
        return this.score;
    }

    int getId() {
        return this.myId;
    }

    int getCurrentScore() {
        int score = 0;
        for (int i = 0; i < this.width * this.height; ++i) {
            if (this.currentState[i] == Block.BoxOnX.ordinal()) {
                ++score;
            }
        }
        return score;
    }

    void setScore(int score) {
        this.score = score;
    }
}
