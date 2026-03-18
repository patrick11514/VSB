static int x = 0;
static int y = 0;
static int MAX_WIDTH = 80;
static int MAX_HEIGHT = 25;

// io.h
unsigned char port_byte_in(unsigned short port)
{
    unsigned char result;
    __asm__ volatile("in %%dx, %%al" : "=a"(result) : "d"(port));
    return result;
}

void port_byte_out(unsigned short port, unsigned char data)
{
    __asm__ volatile("out %%al, %%dx" : : "a"(data), "d"(port));
}

//////

void get_cursor()
{
    port_byte_out(0x3D4, 0x0E);
    unsigned char cursor_high = port_byte_in(0x3D5);
    port_byte_out(0x3D4, 0x0F);
    unsigned char cursor_low = port_byte_in(0x3D5);
    x = cursor_low;
    y = cursor_high;
}

void scroll()
{
    char *voidMemory = (char *)0xb8000;
    for (int i = 0; i < (MAX_HEIGHT - 1) * MAX_WIDTH; i++)
    {
        voidMemory[2 * i] = voidMemory[2 * (i + MAX_WIDTH)];
    }
    for (int i = (MAX_HEIGHT - 1) * MAX_WIDTH; i < MAX_HEIGHT * MAX_WIDTH; i++)
    {
        voidMemory[2 * i] = ' ';
    }
    y--;
}

void print_char(char c)
{
    if (x >= MAX_WIDTH)
    {
        x = 0;
        y++;
    }

    if (y >= MAX_HEIGHT)
    {
        scroll();
    }

    if (c == '\n')
    {
        x = 0;
        y++;
        return;
    }

    char *voidMemory = (char *)0xb8000;
    voidMemory += 2 * (y * MAX_WIDTH + x);
    *voidMemory = c;

    x++;
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
    print_string("            *\n");
    print_string("              .  *\n");
    print_string("          * .\n");
    print_string("            * .  *\n");
    print_string("         *\n");
    print_char('\n');
    print_string("         PleiadOS\n");
    print_string("        Welcome...\n");
}