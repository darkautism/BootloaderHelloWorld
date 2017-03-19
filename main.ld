ENTRY(main);
SECTIONS
{
    . = 0x8200;
    .text : AT(0x8200)
    {
        *(__start);
        _text = .;
        *(.text);
        _text_end = .;
    }
    .data :
    {
        _data = .;
        *(.bss);
        *(.bss*);
        *(.data);
        *(.rodata*);
        *(COMMON)
        _data_end = .;
    }
}