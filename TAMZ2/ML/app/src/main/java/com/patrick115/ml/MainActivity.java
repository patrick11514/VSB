package com.patrick115.ml;

import android.content.ContentValues;
import android.content.Context;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Matrix;
import android.graphics.Paint;
import android.graphics.Path;
import android.graphics.PointF;
import android.graphics.Rect;
import android.graphics.RectF;
import android.graphics.drawable.BitmapDrawable;
import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.Toast;

import androidx.activity.EdgeToEdge;
import androidx.activity.result.ActivityResultLauncher;
import androidx.activity.result.contract.ActivityResultContracts;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.graphics.Insets;
import androidx.core.view.ViewCompat;
import androidx.core.view.WindowInsetsCompat;

import com.google.android.gms.tasks.Task;
import com.google.mlkit.vision.common.InputImage;
import com.google.mlkit.vision.face.Face;
import com.google.mlkit.vision.face.FaceContour;
import com.google.mlkit.vision.face.FaceDetection;
import com.google.mlkit.vision.face.FaceDetector;
import com.google.mlkit.vision.face.FaceDetectorOptions;

import java.io.File;
import java.io.IOException;
import java.io.OutputStream;
import java.util.List;

enum ActivityResults  {
    GALLERY_PICK,
    SHOOT
};

public class MainActivity extends AppCompatActivity {

    List<Face> faces = null;
    FaceDetectorOptions detectionOptions;
    Bitmap imageData = null;

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

        final FaceDetectorOptions[] realtime = {new FaceDetectorOptions.Builder()
                .setContourMode(FaceDetectorOptions.CONTOUR_MODE_ALL)
                .setPerformanceMode(FaceDetectorOptions.PERFORMANCE_MODE_ACCURATE) // Accurate over fast
                .build()};
        FaceDetectorOptions highAccuracyOpts =
                new FaceDetectorOptions.Builder()
                        .setPerformanceMode(FaceDetectorOptions.PERFORMANCE_MODE_ACCURATE)
                        .setLandmarkMode(FaceDetectorOptions.LANDMARK_MODE_ALL)
                        .setClassificationMode(FaceDetectorOptions.CLASSIFICATION_MODE_ALL)
                        .build();

        final boolean[] isRealtime = {false};

        this.detectionOptions = highAccuracyOpts;

        Button gallery = findViewById(R.id.gallery);
        gallery.setOnClickListener(v -> {
            Intent imagePick = new Intent(Intent.ACTION_PICK);
            imagePick.setType("image/*");
            startActivityForResult(imagePick, ActivityResults.GALLERY_PICK.ordinal());
        });

        Button detect = findViewById(R.id.detect);
        detect.setOnClickListener(v -> {
            detect();
        });

        ImageView image = findViewById(R.id.image);
        Paint cyan = new Paint();
        cyan.setColor(Color.CYAN);
        cyan.setStrokeWidth(10);
        cyan.setStyle(Paint.Style.STROKE);

        Button showFaces = findViewById(R.id.faces);
        showFaces.setOnClickListener(v -> {
            if (this.imageData == null) {
                BitmapDrawable bitmapDrawable = (BitmapDrawable) image.getDrawable();
                this.imageData = bitmapDrawable.getBitmap();
            }

            Bitmap newBitmap = this.imageData.copy(Bitmap.Config.ARGB_8888, true);
            Canvas canvas = new Canvas(newBitmap);

            for (Face face : this.faces) {
                Log.i("countours", String.valueOf(face.getAllContours().size()));
                canvas.drawRect(face.getBoundingBox(), cyan);
                for (FaceContour contour : face.getAllContours()) {
                    Path path = new Path();

                    PointF start = null;

                    float xAvg = 0;
                    float yAvg = 0;
                    int count = 0;

                    for (PointF point : contour.getPoints()) {
                        if (start == null) {
                            path.moveTo(point.x, point.y);
                            start = point;
                        } else {
                            path.lineTo(point.x, point.y);
                        }
                        xAvg += point.x;
                        yAvg += point.y;
                        count += 1;
                    }

                    if (contour.getFaceContourType() == FaceContour.FACE) {
                        path.lineTo(start.x, start.y); //complete full face circle
                    }

                    canvas.drawPath(path, cyan);

                    xAvg /= count;
                    yAvg /= count;
                    canvas.drawRect(xAvg - 0.5f, yAvg - 0.5f, xAvg + 0.5f, yAvg + 0.5f, cyan);
                }
            }

            image.setImageBitmap(newBitmap);
        });

        Button mode = findViewById(R.id.quality);
        mode.setOnClickListener(v -> {
            if (isRealtime[0]) {
                detectionOptions = highAccuracyOpts;
                mode.setText("LOW");
            } else {
                detectionOptions = realtime[0];
                mode.setText("HIGH");
            }

            isRealtime[0] = !isRealtime[0];
        });

        Bitmap uwuFace = BitmapFactory.decodeResource(getResources(), R.drawable.uwu);

        Button uwu = findViewById(R.id.uwu);
        uwu.setOnClickListener(v -> {
            if (this.imageData == null) {
                BitmapDrawable bitmapDrawable = (BitmapDrawable) image.getDrawable();
                this.imageData = bitmapDrawable.getBitmap();
            }

            Bitmap newBitmap = this.imageData.copy(Bitmap.Config.ARGB_8888, true);
            Canvas canvas = new Canvas(newBitmap);

            for (Face face : this.faces) {
                List<FaceContour> contours = face.getAllContours();

                if (contours.isEmpty()) {
                    //simply put into the rect
                    Rect faceRect = face.getBoundingBox();
                    canvas.drawBitmap(uwuFace, null, faceRect, null);
                } else {
                    float rotation = face.getHeadEulerAngleZ();


                    /*PointF leftEye = new PointF();
                    PointF rightEye = new PointF();
                    PointF lipTop = new PointF();
                    PointF lipBottom = new PointF();

                    for (FaceContour contour : contours) {
                        if (contour.getFaceContourType() != FaceContour.LEFT_EYE &&
                            contour.getFaceContourType() != FaceContour.RIGHT_EYE &&
                            contour.getFaceContourType() != FaceContour.UPPER_LIP_TOP &&
                            contour.getFaceContourType() != FaceContour.LOWER_LIP_BOTTOM) continue; // we only want eyes

                        float xAvg = 0;
                        float yAvg = 0;
                        int count = 0;

                        for (PointF point : contour.getPoints()) {
                            xAvg += point.x;
                            yAvg += point.y;
                            count += 1;
                        }

                        xAvg /= count;
                        yAvg /= count;

                        if (contour.getFaceContourType() == FaceContour.LEFT_EYE) {
                            leftEye.x = xAvg;
                            leftEye.y = yAvg;
                        }  else if(contour.getFaceContourType() == FaceContour.RIGHT_EYE) {
                            rightEye.x = xAvg;
                            rightEye.y = yAvg;
                        } else if (contour.getFaceContourType() == FaceContour.UPPER_LIP_TOP) {
                            lipTop.x = xAvg;
                            lipTop.y = yAvg;
                        } else if (contour.getFaceContourType() == FaceContour.LOWER_LIP_BOTTOM) {
                            lipBottom.x = xAvg;
                            lipBottom.y = yAvg;
                        }
                    }

                    PointF mouthCenter = new PointF();
                    mouthCenter.x = (lipTop.x + lipBottom.x) / 2;
                    mouthCenter.y = (lipTop.y + lipBottom.y) / 2;

                    PointF pointBetweenEyes = new PointF();
                    pointBetweenEyes.x = (leftEye.x + rightEye.x) / 2;
                    pointBetweenEyes.y = (leftEye.y + rightEye.y) / 2;


                    double distance = Math.sqrt(Math.pow(Math.abs(leftEye.x - rightEye.x), 2) + Math.pow(Math.abs(leftEye.y - rightEye.y), 2));
                    double eyeMouthDistance = Math.sqrt(Math.pow(Math.abs(pointBetweenEyes.x - mouthCenter.x), 2) + Math.pow(Math.abs(pointBetweenEyes.y - mouthCenter.y), 2));

                    //now scale the image based on eyes
                    int DEFAULT_EYE_DISTANCE = 218; //distance of eyes in default image
                    int DEFAULT_EYES_MOUTH_DISTANCE = 70; //distance from eyes to mouth
                    PointF mouthOriginal = new PointF(265, 298);
                    PointF eyeCenterOriginal = new PointF(257, 227);

                    double widthPercentage = distance / (double)DEFAULT_EYE_DISTANCE;
                    double heightPercentage = eyeMouthDistance / (double)DEFAULT_EYES_MOUTH_DISTANCE;

                    Bitmap scaledUwU = Bitmap.createScaledBitmap(uwuFace, (int)(widthPercentage * uwuFace.getWidth()), (int)(heightPercentage * uwuFace.getHeight()), false);*/

                    Matrix rotationMatrix = new Matrix();
                    rotationMatrix.postRotate(rotation * -1, uwuFace.getWidth() / 2f, uwuFace.getHeight() / 2f);
                    Bitmap tiltedUwU = Bitmap.createBitmap(uwuFace, 0,0, uwuFace.getWidth(), uwuFace.getHeight(), rotationMatrix, false);

                    //canvas.drawBitmap(scaledUwU, null, face.getBoundingBox(), null);
                    canvas.drawBitmap(tiltedUwU, null, face.getBoundingBox(), null);

                }
            }

            image.setImageBitmap(newBitmap);
        });

        Button take = findViewById(R.id.take);
        take.setOnClickListener(v -> {
            Intent imagePick = new Intent(MediaStore.ACTION_IMAGE_CAPTURE);
            startActivityForResult(imagePick, ActivityResults.SHOOT.ordinal());
        });

        Button save = findViewById(R.id.save);
        save.setOnClickListener(v -> {
            if (saveToFile()) {
                Toast.makeText(this, "Obrázek byl uložen", Toast.LENGTH_SHORT).show();
            } else {
                Toast.makeText(this, "Obrázek se nepovedlo uložit", Toast.LENGTH_SHORT).show();
            }
        });
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, @Nullable Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        if (requestCode == ActivityResults.GALLERY_PICK.ordinal() && resultCode == RESULT_OK) {
            if (data == null || data.getData() == null) return;

            ImageView img = findViewById(R.id.image);
            try {
                Uri imgUri = data.getData();
                Bitmap imgData = MediaStore.Images.Media.getBitmap(getContentResolver(), imgUri);
                this.imageData = imgData;
                img.setImageBitmap(imgData);

                findViewById(R.id.faces).setEnabled(false);
                findViewById(R.id.uwu).setEnabled(false); // disable and enable it after detecting faces
            } catch (Exception e) {
                e.printStackTrace();
            }
        } else if (requestCode == ActivityResults.SHOOT.ordinal() && resultCode == RESULT_OK) {
            if (data == null) return;

            Bitmap imgData = (Bitmap) data.getExtras().get("data");

            ImageView img = findViewById(R.id.image);
            this.imageData = imgData;
            img.setImageBitmap(imgData);

            findViewById(R.id.faces).setEnabled(false);
            findViewById(R.id.uwu).setEnabled(false); // disable and enable it after detecting faces
        }
    }

    private void detect() {
        if (this.imageData == null) {
            ImageView img = findViewById(R.id.image);
            BitmapDrawable drawable = (BitmapDrawable) img.getDrawable();
            this.imageData = drawable.getBitmap();
        }
        InputImage image = InputImage.fromBitmap(this.imageData, 0);

        FaceDetector detector = FaceDetection.getClient(detectionOptions);

        Task<List<Face>> task = detector.process(image);
        task.addOnSuccessListener(faceList -> {
            if (faceList.isEmpty()) {
                Toast.makeText(this, "Nepovedlo se najít žádný obliček :(", Toast.LENGTH_SHORT).show();
                return;
            }
            faces = faceList;

            //enable buttons
            findViewById(R.id.faces).setEnabled(true);
            findViewById(R.id.uwu).setEnabled(true);
        }).addOnFailureListener(e -> {
                Log.e("FaceDetection", "Fail", e);
        });
    }

    public boolean saveToFile() {
        ImageView img = findViewById(R.id.image);
        BitmapDrawable drawable = (BitmapDrawable) img.getDrawable();
        Bitmap bitmap = drawable.getBitmap();

        Context context = this.getApplicationContext();

        // Define file information
        ContentValues values = new ContentValues();
        values.put(MediaStore.MediaColumns.DISPLAY_NAME, "face_detect.jpg");
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
