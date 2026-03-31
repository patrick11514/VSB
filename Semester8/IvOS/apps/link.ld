ENTRY(entry)

SECTIONS
{
    . = 0x2000;

    .text :
    {
        *(.text.entry)
        *(.text*)
    }

    .rodata :
    {
        *(.rodata*)
    }

    .data :
    {
        *(.data*)
    }

    .bss :
    {
        *(.bss*)
    }
}
