	segment	CODE
	.assume adl=1

    XREF __g_heap_start
    XREF __g_heap_end
    XREF __heapbot
    XREF __heaptop

	XDEF __get_heap_values

__get_heap_values:
    push ix                    ; Standard prologue
    ld ix, 0
    add ix, sp

    ld hl, __heapbot
	ld (__g_heap_start), hl
	ld hl, __heaptop
	ld (__g_heap_end), hl
    ld sp, ix
    pop ix
    ret
