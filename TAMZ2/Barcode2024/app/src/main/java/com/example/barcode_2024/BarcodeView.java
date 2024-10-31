package com.example.barcode_2024;


import android.annotation.SuppressLint;
import android.content.ContentValues;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.AttributeSet;
import android.util.Log;
import android.view.View;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;

import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStream;

public class BarcodeView extends View {

    //UPC-A code

    //http://en.wikipedia.org/wiki/EAN_code
    //http://www.terryburton.co.uk/barcodewriter/generator/


    static final int[] L = {
            0x0D,  //000 1101
            0x19,  //001 1001
            0x13,  //001 0011
            0x3D,  //011 1101
            0x23,  //010 0011
            0x31,  //011 0001
            0x2F,  //010 1111
            0x3B,  //011 1011
            0x37,  //011 0111
            0x0B   //000 1011
    };

    static final int[] R = {
            0x72, //111 0010
            0x66, //110 0110
            0x6C, //110 1100
            0x42, //100 0010
            0x5C, //101 1100
            0x5E, //100 1110
            0x50, //101 0000
            0x44, //100 0100
            0x48, //100 1000
            0x74  //111 0100
    };

    int BARCODE_WIDTH =  600;
    int BARCODE_HEIGHT = 200;
    final static int BARCODE_LINE_WIDTH = 5;

    // čísla čárového kódu
    int[] code = new int[12];

    private float offset = 0;
    private float lineWidth = 0;
    private int height;
    private StringBuilder sb = new StringBuilder();

    public BarcodeView(Context context) {
        super(context);
        setDefaults();
    }

    public BarcodeView(Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        loadArguments(context, attrs);
    }

    public BarcodeView(Context context, @Nullable AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        loadArguments(context, attrs);
    }

    public BarcodeView(Context context, @Nullable AttributeSet attrs, int defStyleAttr, int defStyleRes) {
        super(context, attrs, defStyleAttr, defStyleRes);
        loadArguments(context, attrs);
    }

    private void loadArguments(Context context, AttributeSet attrs) {
        if (attrs != null) {
            TypedArray a = context.getTheme().obtainStyledAttributes(attrs, com.example.barcode_2024.R.styleable.BarcodeView, 0, 0);
            try {
                this.setCode(a.getString(com.example.barcode_2024.R.styleable.BarcodeView_code));
            } finally {
                a.recycle();
            }
        }
    }

    @Override
    protected void onMeasure(int widthMeasureSpec, int heightMeasureSpec) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec);
    }

    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        super.onSizeChanged(w, h, oldw, oldh);
        // při změně velikosti view,  w a h obsahují novou velikost
        this.BARCODE_WIDTH = w;
        this.BARCODE_HEIGHT = h;
    }

    // nastaví výchozí hodnoty
    void setDefaults() {
        int[] copyFrom = {1,2,3,4,5,6,7,8,9,0,1,2};
        System.arraycopy(copyFrom, 0, code, 0, copyFrom.length);
    }

    @SuppressLint("DrawAllocation")
    @Override
    protected void onDraw(@NonNull Canvas canvas) {
        super.onDraw(canvas);

        Paint twPaint = new Paint();
        twPaint.setColor(Color.WHITE);

        Paint tbPaint = new Paint();
        tbPaint.setColor(Color.BLACK);

        Paint trPaint = new Paint();
        trPaint.setColor(Color.RED);
        trPaint.setStrokeWidth(BARCODE_LINE_WIDTH);

        // vykreslí bílý obdelník do kterého se bude kreslit čárový kód
        canvas.drawRect(new Rect(0, 0, BARCODE_WIDTH, BARCODE_HEIGHT), twPaint);

        // tloušťka čáry
        tbPaint.setStrokeWidth(BARCODE_LINE_WIDTH);

        // velikost písma, antialiasing
        trPaint.setTextSize(30);
        trPaint.setAntiAlias(true);

        //canvas.drawLine(0, 0, BARCODE_WIDTH, BARCODE_HEIGHT, tbPaint);
        //canvas.drawLine(0, BARCODE_HEIGHT, BARCODE_WIDTH, 0, tbPaint);

        //canvas.drawText("Zde bude čárový kód", (int)(BARCODE_WIDTH * 0.3), (int)(BARCODE_HEIGHT * 0.95), trPaint);

        int quietZones = 9 * 2;
        int startEnd = 3 * 2;
        int middle = 5;
        int leftRight = (6 * 7) * 2;

        //Calculate width of each line
        this.lineWidth = BARCODE_WIDTH * ((float)1 / (quietZones + startEnd + middle + leftRight));

        drawQR(canvas, tbPaint, twPaint);

        Log.i("QR", sb.toString());
    }

    private void drawQR(Canvas canvas, Paint black, Paint white) {
        this.offset = 0;
        this.sb = new StringBuilder();

        //quitet left
        drawQuiet(canvas, white);
        sb.append('|');
        //start
        drawStartOrEnd(canvas, black, white);
        sb.append('|');
        //left zone
        drawZone(canvas, black, white, true);
        sb.append('|');
        //middle
        drawMiddle(canvas, black, white);
        sb.append('|');
        //right zone
        drawZone(canvas, black, white, false);
        sb.append('|');
        //end
        drawStartOrEnd(canvas, black, white);
        sb.append('|');
        //quitet right
        drawQuiet(canvas, white);
    }

    private void drawLine(Canvas canvas, int pos, Paint color) {
        canvas.drawRect(this.offset + (pos * this.lineWidth), 0, this.offset + (pos * this.lineWidth) + this.lineWidth, BARCODE_HEIGHT, color);
    }

    private void drawQuiet(Canvas canvas, Paint paint) {
        int width = 9;
        for (int i = 0; i < width; ++i) {
            drawLine(canvas, i, paint);
            sb.append('0');
        }
        this.offset += width * this.lineWidth;
    }

    private void drawStartOrEnd(Canvas canvas, Paint black, Paint white) {
        int width = 3;
        for (int i = 0; i < width; ++i) {
            drawLine(canvas, i, i % 2 == 0 ? black : white);
            sb.append(i % 2 == 0 ? 1 : 0);
        }
        this.offset += width * this.lineWidth;
    }

    private void drawZone(Canvas canvas, Paint black, Paint white, boolean left) {
        int segments = 6;
        int width = 7;
        for (int i = 0; i < segments; ++i) {
            int number = code[(left ? 0 : segments) + i];
            int code;
            Log.i("NUM", String.valueOf(number));
            if (left)
                code = L[number]; // get number reprezentation
            else
                code = R[number];


            Log.i("HEXA", String.format("0x%08X", number));

            for (int l = 0; l < width; ++l) {
                drawLine(canvas, l, ((code >> width - l - 1) & 1) == 1 ? black : white);
                Log.i("QR", "Drawing " + number + " Side: " + left + " Index: " + l + " Shift: " + (width - l - 1) + " Result: " + (((code >> width - l - 1) & 1) == 1));
                sb.append(((code >> width - l - 1) & 1) == 1 ? 1 : 0);
            }
            this.offset += width * this.lineWidth;
        }
    }
    private void drawMiddle(Canvas canvas, Paint black, Paint white) {
        int width = 5;
        for (int i = 0; i < width; ++i) {
            drawLine(canvas, i, i % 2 == 0 ? white : black);
            sb.append(i % 2 == 0 ? 0 : 1);

        }
        this.offset += width * this.lineWidth;
    }

    public String getCode() {
        StringBuilder sb = new StringBuilder();
        for (int value : this.code) {
            sb.append(value);
        }
        return sb.toString();
    }

    private int getLast(String code) {
        int lastDigit = 0;
        for (int i = 0; i < 11; ++i) {
            int c = Character.getNumericValue(code.charAt(i));
            lastDigit += ((i % 2 == 0) ? 3 : 1) * c;
        }
        lastDigit %= 10;

        return 10 - lastDigit;
    }

    private boolean isCorrect() {
        int sum = 0;
        for (int i = 0; i < 12; ++i) {
            sum += ((i % 2 == 0) ? 3 : 1) * this.code[i];
        }
        return sum % 10 == 0;
    }

    public int setCode(String code) {
        //dont set invalid values
        if (code.length() < 11 || code.length() > 12) return -1;

        if (code.length() == 11) {
            //calculate 12th digit
            code += String.valueOf(this.getLast(code));
        }

        for (int i = 0; i < 12; ++i) {
            int c = Character.getNumericValue(code.charAt(i));
            this.code[i] = c;
        }

        invalidate(); // Redraw the view if necessary

        //if control digit is corret, return it, otherwise calculate correct
        return this.isCorrect() ? this.code[11] : this.getLast(code);
    }

    public boolean saveToFile() {
        Bitmap bitmap = Bitmap.createBitmap(this.getWidth(), this.getHeight(), Bitmap.Config.ARGB_8888);
        Canvas canvas = new Canvas(bitmap);
        this.draw(canvas);

        StringBuilder sb = new StringBuilder();

        for (int digit : this.code) {
            sb.append(digit);
        }

        Context context = getContext();

        // Define file information
        ContentValues values = new ContentValues();
        values.put(MediaStore.MediaColumns.DISPLAY_NAME, "code_" + sb + ".jpg");
        values.put(MediaStore.MediaColumns.MIME_TYPE, "image/jpeg");
        values.put(MediaStore.MediaColumns.RELATIVE_PATH, Environment.DIRECTORY_PICTURES);

        Uri fileUri = context.getContentResolver().insert(MediaStore.Images.Media.EXTERNAL_CONTENT_URI, values);
        if (fileUri == null) {
            return false; // Failed to get Uri, return false
        }

        try (OutputStream out = context.getContentResolver().openOutputStream(fileUri)) {
            if (out != null) {
                // Compress and write the bitmap to the OutputStream
                boolean saved = bitmap.compress(Bitmap.CompressFormat.JPEG, 100, out);
                out.flush();
                return saved; // Return true if saved successfully
            }
        } catch (Exception e) {
            Log.e("File Write Error", e.toString());
        }

        return false; // Return false if saving failed
    }
}

