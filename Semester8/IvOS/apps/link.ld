ENTRY(entry)

SECTIONS
{
    . = 0x100000;

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
