package cz.patrick115.soko2;


import android.annotation.SuppressLint;
import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

import kotlin.Pair;
import kotlin.Triple;


/**
 * Created by kru13
 */
public class SokoView extends View implements ViewEventHandler{

    Bitmap[] bmp;

    int width;
    int height;

    // 0 = empty
    // 1 = wall
    // 2 = block
    // 3 = X
    // 4 = player
    // 5 = block on X

    float touchStartX;
    float touchStartY;

    int playerX;
    int playerY;
    Block underPlayer = Block.Floor;
    boolean playable = true;

    private Level level;
    private ViewEventHandler eventHandler;

    private Block getBlock(int x, int y) {
        return Block.values()[level.at(x, y)];
    }

    //offsetX and offsetY will always be -1, 0 or 1
    private boolean canMove(int offsetX, int offsetY) {
        Block blockInDirection = getBlock(playerX + offsetX, playerY + offsetY);
        switch (blockInDirection) {
            case Floor:
            case X:
                return true;
            case Box:
            case BoxOnX:
                //get block in same direction
                Block block = getBlock(playerX + offsetX * 2, playerY + offsetY * 2);
                if (block == Block.Wall || block == Block.Box || block == Block.BoxOnX) return false;

                if (block == Block.Floor) {
                    //move box
                    placeBlock(playerX + offsetX, playerY + offsetY, blockInDirection == Block.Box ? Block.Floor : Block.X);
                    placeBlock(playerX + offsetX * 2, playerY + offsetY * 2, Block.Box);

                    checkWin();
                    return true;
                } else if (block == Block.X) {
                    placeBlock(playerX + offsetX, playerY + offsetY, blockInDirection == Block.Box ? Block.Floor : Block.X);
                    placeBlock(playerX + offsetX * 2, playerY + offsetY * 2, Block.BoxOnX);

                    checkWin();
                    return true;
                }

        }

        return false;
    }

    private void placeBlock(int x, int y, Block block) {
        level.set(x, y, block);
    }

    private void move(int offsetX, int offsetY) {
        if (!canMove(offsetX, offsetY)) return;

        //calcualte boxes
        int currentScore = this.level.getCurrentScore();
        if (currentScore > this.level.getScore()) {
            this.level.setScore(currentScore);
            if (this.eventHandler != null) {
                this.eventHandler.onUpdate(currentScore);
            }
        }

        //replace player with block under them
        placeBlock(playerX, playerY, underPlayer);

        //move player coords
        playerX = playerX + offsetX;
        playerY = playerY + offsetY;

        //save block which will be under player
        underPlayer = getBlock(playerX, playerY);

        //player player
        placeBlock(playerX, playerY, Block.Player);

        //rerender
        invalidate();
    }

    private void moveRight() {
        move(1, 0);
    }

    private void moveLeft() {
        move(-1, 0);
    }

    private void moveUp() {
        move(0, -1);
    }

    private void moveDown() {
        move(0, 1);
    }

    private void resetLevel() {
        if (this.level == null) return;
        this.level.reset();

        Triple<Integer, Integer, Boolean> player = level.getPlayer();
        this.playerX = player.getFirst();
        this.playerY = player.getSecond();
        this.underPlayer = player.getThird() ? Block.X : Block.Floor;

        invalidate();
    }

    private void checkWin() {
        if (!this.level.win()) return;
        
        //no regular boxes found
        if (this.eventHandler != null) {
            //notify to change level
            this.eventHandler.onUpdate();
        }
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (!playable) return super.onTouchEvent(event);
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            touchStartX = event.getX();
            touchStartY = event.getY();
            return true;
        } else if (event.getAction() == MotionEvent.ACTION_UP) {
            float deltaX = event.getX() - touchStartX;
            float deltaY = event.getY() - touchStartY;

            if (deltaX > 500 && deltaY > 1000) {
                resetLevel();
                return true;
            }

            //move horizontally
            if (Math.abs(deltaX) > Math.abs(deltaY)) {
                if (deltaX > 0) {
                    moveRight();
                } else {
                    moveLeft();
                }
            } else {
                //move vertically
                if( deltaY > 0) {
                    moveDown();
                } else {
                    moveUp();
                }
            }
            return true;
        }
        return super.onTouchEvent(event);
    }

    public SokoView(Context context) {
        super(context);
        init(context);
    }

    public SokoView(Context context, AttributeSet attrs) {
        super(context, attrs);
        loadArguments(context, attrs);
        init(context);
    }

    public SokoView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        loadArguments(context, attrs);
        init(context);
    }

    public void bind(Level level) {
        this.level = level;

        Triple<Integer, Integer, Boolean> player = level.getPlayer();
        this.playerX = player.getFirst();
        this.playerY = player.getSecond();
        if (player.getThird()) {
            this.underPlayer = Block.X;
        }

        if (this.eventHandler != null) {
            this.eventHandler.onUpdate(level.getScore());
        }

        invalidate();
    }

    private void loadArguments(Context context, AttributeSet attrs) {
        if (attrs != null) {
            TypedArray a = context.getTheme().obtainStyledAttributes(attrs, R.styleable.SokoView, 0, 0);
            try {
                this.playable = a.getBoolean(R.styleable.SokoView_playable, true);
            } finally {
                a.recycle();
            }
        }
    }

    void init(Context context) {
        bmp = new Bitmap[6];

        bmp[0] = BitmapFactory.decodeResource(getResources(), R.drawable.empty);
        bmp[1] = BitmapFactory.decodeResource(getResources(), R.drawable.wall);
        bmp[2] = BitmapFactory.decodeResource(getResources(), R.drawable.box);
        bmp[3] = BitmapFactory.decodeResource(getResources(), R.drawable.goal);
        bmp[4] = BitmapFactory.decodeResource(getResources(), R.drawable.hero);
        bmp[5] = BitmapFactory.decodeResource(getResources(), R.drawable.boxok);

        resetLevel();
    }

    public void addEventHandler(ViewEventHandler eventHandler) {
        this.eventHandler = eventHandler;
    }


    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        width = w;
        height = h;
        super.onSizeChanged(w, h, oldw, oldh);
    }

    //@SuppressLint("DrawAllocation")
    @SuppressLint("DrawAllocation")
    @Override
    protected void onDraw(Canvas canvas) {
        if (this.level == null) return;
        Pair<Integer, Integer> size = this.level.getSize();

        Paint tbPaint = new Paint();
        tbPaint.setColor(Color.BLACK);

        canvas.drawRect(new Rect(0,0, width, height), tbPaint);

        int rectWidth = width / size.getFirst();
        int rectHeight = height / size.getSecond();

        int side = Math.min(rectWidth, rectHeight);

        for (int y = 0; y < size.getSecond(); y++) {
            for (int x = 0; x < size.getFirst(); x++) {
                canvas.drawBitmap(bmp[level.at(x, y)], null,
                        new Rect(x*side,
                                y*side,
                                (x+1)*side,
                                (y+1)*side), null);
            }
        }

    }

    @Override
    public void onUpdate(int data) {
        //EMPTY
    }

    @Override
    public void onUpdate() {
        this.invalidate();
    }
}
