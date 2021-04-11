;	.include "kernel.inc"
;	.include "ez80F91.inc"


	.assume ADL=1
	.extern _spi_rx_ptr
	.extern _spi_rx_buf
	.extern _spi_tx_cnt
	.extern _spi_rx_cnt
	.extern _e_spi_state

	.def	_spi_pin_isr
	
   NOP
_spi_pin_isr:
	PUSH	AF
	
	; Disable PB5 interrupt
	IN0		A,	(009DH)
	RES		%5,A
    OUT0	(009DH), A
	PUSH	BC
	LD	BC,_spi_rx_buf
	LD	(_spi_rx_ptr),BC
	LD	A,%1
	LD	(_spi_tx_cnt),A
	LD	A,%5
	LD	(_spi_rx_cnt),A
	LD	BC,0
	LD	(_e_spi_state),BC
 	POP		BC
	;SELECT SLAVE PIN
	IN0		A,	(009AH)
	RES		%4,A
	OUT0	(009AH), A
	;TRANSMIT CMD BYTE
	LD		A,%1
	OUT0	(00BCH), A
	POP		AF
	EI
	RETI
