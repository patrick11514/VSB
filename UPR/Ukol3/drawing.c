#include <stdio.h>
#include <unistd.h>

// ANSI escape sequences reference:
// - https://www.lihaoyi.com/post/BuildyourownCommandLinewithANSIescapecodes.html#cursor-navigation
// - https://tldp.org/HOWTO/Bash-Prompt-HOWTO/x361.html

// Height of the drawing canvas
int canvas_height = 40;

//---Cursor management---

// Hide the blinking terminal cursor.
void hide_cursor();

// Show the terminal cursor.
void show_cursor();

//---Color management---

// Reset the drawing and background color to the default values.
void reset_color();

// Set the background color to red.
void set_red_color();

// Set the background color to green.
void set_green_color();

// Set the background color to blue.
void set_blue_color();

//---Screen management---

// Reset the drawing environment.
// Clears the screen, moves the cursor to the origin in the top-left corner and
// prepares the drawing canvas.
void clear_screen();

//---Cursor movement---

// Move cursor right.
void move_right();

// Move cursor left.
void move_left();

// Move cursor up.
void move_up();

// Move cursor down.
void move_down();

// Move cursor to the specified `line` and `column` position.
void move_to(int line, int column);

//---Drawing---

// Draws a single "pixel" using the currently set background color.
// The cursor position is not changed after drawing!
// You need to move the cursor manually before drawing the next pixel,
// otherwise you will just overwrite the previous pixel.
void draw_pixel();

// Clears the drawing session, resets colors and cursor.
// Call this before the end of the program, otherwise your terminal will get messed up.
void end_drawing();

// Pause the program for a while, so that you can see what has been drawn so far.
// If you repeatedly call animate() after drawing something, you can crate interactive animations.
// You can also combine this with clear_screen() to draw something new on every "animation frame".
void animate();

//----------------------------
// Put your functions here

// create house
void roof()
{
    // roof
    int width = 1;
    for (int i = 0; i < 4; i++)
    {
        for (int l = 0; l < width; l++)
        {
            draw_pixel();
            move_right();
        }
        width += 2;
        move_down();
        for (int l = 0; l < width - 1; l++)
        {
            move_left();
        }
    }
}

void chimney()
{
    for (int i = 0; i < 2; i++)
    {
        move_right();
        move_up();
    }

    // chimney
    for (int i = 0; i < 2; i++)
    {
        move_up();
        draw_pixel();
    }
}

void body()
{
    for (int i = 0; i < 4; i++)
    {
        move_down();
    }
    // body
    for (int i = 0; i < 3; i++)
    {
        for (int l = 0; l < 5; l++)
        {
            draw_pixel();
            move_right();
        }
        move_down();
        for (int l = 0; l < 5; l++)
        {
            move_left();
        }
    }
}

void house()
{
    // draw roof
    set_blue_color();
    roof();

    // draw chimney
    set_red_color();
    chimney();

    set_green_color();
    body();

    // back to start line to be able to draw another house
    for (int i = 0; i < 10; i++)
    {
        move_right();
        if (i < 7)
            move_up();
    }
}

void village(int houses)
{
    int current = 1;
    move_to(current, 5);
    for (int i = 0; i < houses; i++)
    {
        // new line
        if (i % 12 == 0 && i > 0)
        {
            current += 9;
            move_to(current, 5);
        }
        house();
    }
}

int main()
{
    // Keep this line here
    clear_screen();

    // Put your drawing code here
    village(20);

    // Keep this line here
    end_drawing();

    return 0;
}

//----------------------------

void hide_cursor()
{
    printf("\x1b[?25l");
}
void show_cursor()
{
    printf("\x1b[?25h");
}

void reset_color()
{
    printf("\x1b[0m");
}
void set_red_color()
{
    printf("\x1b[41;1m");
}
void set_green_color()
{
    printf("\x1b[42;1m");
}
void set_blue_color()
{
    printf("\x1b[44;1m");
}

void clear_screen()
{
    reset_color();
    // hide_cursor();

    // Clear screen
    printf("\x1b[2J");
    // Move cursor to (0, 0)
    printf("\x1b[0;0F");
    // Create canvas
    for (int i = 0; i < canvas_height; i++)
    {
        printf("\n");
    }
    for (int i = 0; i < canvas_height; i++)
    {
        printf("\x1b[1A");
    }
}

void move_right()
{
    printf("\x1b[1C");
}
void move_left()
{
    printf("\x1b[1D");
}
void move_up()
{
    printf("\x1b[1A");
}
void move_down()
{
    printf("\x1b[1B");
}
void move_to(int line, int column)
{
    printf("\x1b[%d;%dH", line, column);
}

void draw_pixel()
{
    printf(" ");
    move_left();
}
void end_drawing()
{
    reset_color();
    printf("\x1b[0;0H");
    for (int i = 0; i < canvas_height; i++)
    {
        move_down();
    }
    printf("\n");
    show_cursor();
}
void animate()
{
    fflush(stdout);
    // Sleep for 500ms
    usleep(1000 * 500);
}
