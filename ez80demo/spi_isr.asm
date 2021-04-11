;	.include "kernel.inc"
;	.include "ez80F91.inc"


	.assume ADL=1
	.extern _RZKResumeInterruptThread
	.extern _RZKISRProlog
	.extern _RZKISREpilog
	.extern _spi_rx_ptr
	.extern _spi_tx_ptr
	.extern _spi_tx_cnt
	.extern _spi_rx_cnt
	.extern _ghSPI_isr
	
	.def	_spi_isr
	
   NOP
_spi_isr:
	PUSH	AF
	IN0	A,(00BBH);if(SPI_SR&SPIF)
	AND	A,%80
	JR	Z,L_20
	PUSH	BC;{
	PUSH	HL
	LD	A,(_spi_tx_cnt);if(spi_tx_cnt)
	OR	A,A
	JR	Z,L_18
	LD	A,(_spi_tx_cnt);{
	DEC	A				;	spi_tx_cnt--;
	LD	(_spi_tx_cnt),A
	OR	A,A				;	if(spi_tx_cnt)
	JR	Z,L_10
	LD	BC,(_spi_tx_ptr);	{
	INC	BC				;		spi_tx_ptr++;
	LD	(_spi_tx_ptr),BC
	LD	HL,(_spi_tx_ptr);		SPI_TSR==*spi_tx_ptr;
	LD	A,(HL)
	OUT0	(0BCH),A	
	JR	L_15;				}
L_10:	
	LD	(_spi_rx_cnt),A
	OR	A,A				
	JR	Z,L_16;				if(spi_rx_cnt==0) goto task isr
	IN0	A,(09AH);			else
	SET	%4,A;				{
	OUT0	(09AH),A;			SPI_SLAVE_PIN_DESELECT;
	NOP;						{
	NOP
	NOP
	NOP
	NOP
    IN0	A,(09AH);					SPI_SLAVE_PIN_SELECT;
	RES	%4,A
	OUT0	(09AH),A
	LD	A,%AA;						SPI_TSR=0xAA;//read marker
	OUT0	(0BCH),A
	JR	L_15;				}
L_18:
	LD	HL,(_spi_rx_ptr);else
	IN0	A,(0BCH);		{
	LD	(HL),A;				*spi_rx_ptr=SPI_RBR;
	LD	BC,(_spi_rx_ptr);	spi_rx_ptr++;
	INC	BC
	LD	(_spi_rx_ptr),BC
	LD	A,(_spi_rx_cnt)
	DEC	A
	LD	(_spi_rx_cnt),A;	spi_rx_cnt--;
	OR	A,A;				if(spi_rx_cnt)
	JR	Z,L_16;				{
	LD	A,%AA
	OUT0	(0BCH),A;			SPI_TSR=0xAA;//read marker
	JR	L_15;				}
L_16:	
	IN0	A,(09AH);			else
	SET	%4,A;				{
	OUT0	(09AH),A;			SPI_SLAVE_PIN_DESELECT;
	PUSH	DE
	PUSH	IX
	PUSH	IY
	CALL	_RZKISRProlog
	LD		HL, (_ghSPI_isr)
	PUSH	HL
	CALL	_RZKResumeInterruptThread	;Interrupt specific C code
	POP		HL
	CALL	_RZKISREpilog
    POP		IY
	POP     IX;				}
	POP		DE;			}
L_15:
	POP		HL
 	POP		BC
L_20:
	POP		AF
	EI
	RETI
