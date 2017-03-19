ENTRY(main);
SECTIONS
{
    . = 0x8000;
    .text : AT(0x8000)
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
    .fill : AT(0x81FF)
    {
        BYTE(0x00);
    }
}