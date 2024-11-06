package com.tamz.soko2024;


import android.annotation.SuppressLint;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.Canvas;
import android.graphics.Rect;
import android.util.AttributeSet;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Toast;

enum Block {
    Floor,
    Wall,
    Box,
    X,
    Player,
    BoxOnX
};

/**
 * Created by kru13
 */
public class SokoView extends View{

    Bitmap[] bmp;

    int lW = 10;
    int lH = 15;

    int width;
    int height;

    // 0 = empty
    // 1 = wall
    // 2 = block
    // 3 = X
    // 4 = player
    // 5 = block on X

    private int[] baseLevel = {
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            1,1,1,1,1,1,1,1,1,0,
            1,0,0,0,0,0,0,0,1,0,
            1,0,2,3,3,2,1,0,1,0,
            1,0,1,3,2,3,2,0,1,0,
            1,0,2,3,3,2,4,0,1,0,
            1,0,1,3,2,3,2,0,1,0,
            1,0,2,3,3,2,1,0,1,0,
            1,0,0,0,0,0,0,0,1,0,
            1,1,1,1,1,1,1,1,1,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };

    private int[] level = new int[baseLevel.length];

    float touchStartX;
    float touchStartY;

    int playerX = 6;
    int playerY = 7;
    Block underPlayer = Block.Floor;

    private Block getBlock(int x, int y) {
        return Block.values()[level[lW * y + x]];
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
        level[lW * y + x] = block.ordinal();
    }

    private void move(int offsetX, int offsetY) {
        if (!canMove(offsetX, offsetY)) return;

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
        System.arraycopy(baseLevel, 0, level, 0, baseLevel.length);

        playerX = 6;
        playerY = 7;

        invalidate();
    }

    private void checkWin() {
        for (int blockId : level) {
            //some boxes are still there
            if (Block.values()[blockId] == Block.Box) return;
        }
        
        //no regular boxes found
        resetLevel();
        Toast.makeText(getContext(), "Congratulation!!! You did it!", Toast.LENGTH_SHORT).show();
    }

    @Override
    public boolean onTouchEvent(MotionEvent event) {
        if (event.getAction() == MotionEvent.ACTION_DOWN) {
            touchStartX = event.getX();
            touchStartY = event.getY();
            return true;
        } else if (event.getAction() == MotionEvent.ACTION_UP) {
            float deltaX = event.getX() - touchStartX;
            float deltaY = event.getY() - touchStartY;

            Log.i("IDK", deltaX + " " + deltaY);

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
        init(context);
    }

    public SokoView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        init(context);
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


    @Override
    protected void onSizeChanged(int w, int h, int oldw, int oldh) {
        width = w / lW;
        height = h / lH;
        super.onSizeChanged(w, h, oldw, oldh);
    }

    //@SuppressLint("DrawAllocation")
    @SuppressLint("DrawAllocation")
    @Override
    protected void onDraw(Canvas canvas) {

        for (int y = 0; y < lH; y++) {
            for (int x = 0; x < lW; x++) {
                canvas.drawBitmap(bmp[level[y*lW + x]], null,
                        new Rect(x*width,
                                y*height,
                                (x+1)*width,
                                (y+1)*height), null);
            }
        }

    }
}
