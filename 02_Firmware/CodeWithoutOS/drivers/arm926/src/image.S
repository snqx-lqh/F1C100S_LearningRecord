.section ".image_header", "ax"

#if defined (EGON_HEADER)
    .globl _image_start
    _image_start:

    /* Boot head information for BROM */
    b __start
    .byte 'e', 'G', 'O', 'N', '.', 'B', 'T', '0'
    .long 0, __image_end     /* checksum, len */
    .byte 'S', 'P', 'L', 2
    .long 0, 0
    .long 0, 0, 0, 0, 0, 0, 0, 0
    .long 0, 0, 0, 0, 0, 0, 0, 0

#elif defined (LOAD_HEADER)
    .globl _image_start
    _image_start:

    /* Boot head information for BROM */
    b __start
    .long 0x555AAAFF
    .long (__image_start)   /* start */
    .long (__image_end)     /* end */
    .long 0     /* reserved */
    .long 0     /* reserved for CRC */

#else
#error "no define EGON_HEADER or LOAD_HEADER ,please add CFLAGS +=-DLOAD_HEADER for app or CFLAGS +=-DEGON_HEADER"
#endif
