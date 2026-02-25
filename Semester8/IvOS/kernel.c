static int x = 0;
static int y = 0;
static int MAX_WIDTH = 80;
static int MAX_HEIGHT = 25;

void print_char(char c)
{
    if (c == '\n')
    {
        x = 0;
        y++;
        return;
    }

    char *voidMemory = (char *)0xb8000;
    voidMemory += 2 * (y * 80 + x);
    *voidMemory = c;

    x++;
    if (x >= 80)
    {
        x = 0;
        y++;
    }
}

void print_string(const char *str)
{
    while (*str)
    {
        print_char(*str);
        str++;
    }
}

void clear_screen()
{
    for (int i = 0; i < MAX_WIDTH * MAX_HEIGHT; i++)
    {
        char *voidMemory = (char *)0xb8000;
        voidMemory += 2 * i;
        *voidMemory = ' ';
    }
    x = 0;
    y = 0;
}

void kernel_main()
{
    clear_screen();
    print_string("            *\n              .  *\n          * .\n            * .  *\n         *\n\n         PleiadOS\n       Loading kernel...\n");
}