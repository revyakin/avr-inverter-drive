/*******************************************************************************
 * $Id$
 ******************************************************************************/

#include <inttypes.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#define USART_BUFFER_LENGTH 16

#define disable_udre_interrupt() (UCSRB &= ~(1 << UDRIE))
#define enable_udre_interrupt()  (UCSRB |=  (1 << UDRIE))

/* private vars */
uint8_t output_buffer[USART_BUFFER_LENGTH];
uint8_t outbuff_start = 0, outbuff_end = 0;
uint8_t outbuff_overflow = 0;

/* Interrupts */
ISR(USART_RX_vect)
{
}

ISR(USART_TX_vect)
{
} 

ISR(USART_UDRE_vect)
{
	if (!outbuff_overflow && (outbuff_start == outbuff_end))
	{
		disable_udre_interrupt();
	}
		
	outbuff_overflow = 0;
		
	UDR = output_buffer[outbuff_start];
		
	if (++outbuff_start == USART_BUFFER_LENGTH)
		outbuff_start = 0;
}

/* public routines */
void usart_init(uint16_t ubr)
{
	/* Set baud rate */
	UBRRH = (uint8_t)(ubr >> 8);
	UBRRL = (uint8_t) ubr;

	/* Set frame format: 8data, no parity & 2 stop bits */
	UCSRC = (0 << UMSEL)|(0 << UPM0)|(1 << USBS)|(3 << UCSZ0);

	/* Enable receiver and transmitter */
	UCSRB = (1 << RXEN)|(1 << TXEN);
}

void usart_read()
{
}

uint8_t usart_putb(uint8_t data)
{
	if (outbuff_overflow)
		return 0;
		
	output_buffer[outbuff_end] = data;
		
	if (++outbuff_end == USART_BUFFER_LENGTH)
	{
		outbuff_end = 0;
	}
		
	if (outbuff_end == outbuff_start)
	{
		outbuff_overflow = 1;
	}
}
