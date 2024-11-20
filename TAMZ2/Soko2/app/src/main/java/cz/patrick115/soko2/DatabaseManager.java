package cz.patrick115.soko2;

import android.content.ContentValues;
import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.provider.ContactsContract;

public class DatabaseManager {
    private DatabaseHelper dbHelper;

    public DatabaseManager(Context context) {
        dbHelper = new DatabaseHelper(context);
    }

    // Insert data into the database
    public long insertData(int id, int width, int height, int score, String data) {
        SQLiteDatabase db = dbHelper.getWritableDatabase();

        // Use ContentValues to prepare the data
        ContentValues values = new ContentValues();
        values.put(DatabaseHelper.COLUMN_ID, id);
        values.put(DatabaseHelper.COLUMN_WIDTH, width);
        values.put(DatabaseHelper.COLUMN_HEIGHT, height);
        values.put(DatabaseHelper.COLUMN_SCORE, score);
        values.put(DatabaseHelper.COLUMN_DATA, data);

        // Insert the data into the table
        long rowId = db.insert(DatabaseHelper.TABLE_NAME, null, values);
        db.close();
        return rowId;
    }

    public boolean exists(int id) {
        SQLiteDatabase db = dbHelper.getReadableDatabase();
        Cursor cur = db.query(DatabaseHelper.TABLE_NAME, new String[]{DatabaseHelper.COLUMN_ID}, DatabaseHelper.COLUMN_ID + " = ?", new String[]{String.valueOf(id)}, null, null, null);

        if (cur == null) {
            return false;
        }

        if (cur.getCount() == 0) {
            cur.close();
            db.close();
            return false;
        }

        cur.moveToNext();
        int colIndex = cur.getColumnIndex(DatabaseHelper.COLUMN_ID);
        if (colIndex < 0) {
            cur.close();
            db.close();
            return false;
        }
        int gotId = cur.getInt(colIndex);

        cur.close();
        db.close();

        return gotId == id;
    }

    public void update(int id, int width, int height, int score, String data) {
        SQLiteDatabase db = dbHelper.getWritableDatabase();

        ContentValues values = new ContentValues();
        values.put(DatabaseHelper.COLUMN_WIDTH, width);
        values.put(DatabaseHelper.COLUMN_HEIGHT, height);
        values.put(DatabaseHelper.COLUMN_SCORE, score);
        values.put(DatabaseHelper.COLUMN_DATA, data);

        db.update(DatabaseHelper.TABLE_NAME, values, DatabaseHelper.COLUMN_ID + " = ?", new String[]{String.valueOf(id)});

        db.close();
    }

    public Cursor select(int id) {
        SQLiteDatabase db = dbHelper.getReadableDatabase();

        return db.query(DatabaseHelper.TABLE_NAME, null, DatabaseHelper.COLUMN_ID + " = ?", new String[]{String.valueOf(id)}, null, null, null);
    }
}
