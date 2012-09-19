#include "slip.h"
#include "uart/uart.h"

/* SLIP special character codes
*/
#define END             0300    /* indicates start/end of packet */
#define ESC             0333    /* indicates byte stuffing */
#define ESC_END         0334    /* ESC ESC_END means END data byte */
#define ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */

/* Package buffer */
unsigned char package[PKG_MAX_LEN];

int recv_package(void)
{
	/* received bytes counter */
	static int received = 0;
	
	/* escape flag */
	static char escape   = 0;
	
	int tmp;
	
	/* get a character to process */
	int c = uart_getc();
	
	/* if no character available -- return */
	if (c & UART_NO_DATA)
		return 0;
	
	switch(c)
	{
	
	/* if it's an END character then we're done with
	* the packet
	*/
	case END:
		tmp = received;
		received = 0;
		return tmp;
			
	/* if it's the same code as an ESC character, wait
	* and get another character and then figure out
	* what to store in the packet based on that.
	*/
	case ESC:
		escape = 1;
		break;
			
	default:
		
		if (escape)
		{
			escape = 0;
			
			/* if "c" is not one of these two, then we
			* have a protocol violation.  The best bet
			* seems to be to leave the byte alone and
			* just stuff it into the packet
			*/
			switch(c)
			{
			case ESC_ESC:
				c = ESC;
				break;
					
			case ESC_END:
				c = END;
				break;
			}
		}
		
		/* store the character */
		if (received < PKG_MAX_LEN)
		{
			package[received++] = c;
		}
		else
		{
			received = 0;
			return PKG_MAX_LEN;
		}
	}
	
	return 0;
}

void send_package(char len)
{
	int p = 0;
	
//	uart_put(END);
	
	while (len--)
	{
		switch (package[p])
		{
		case END:
			uart_putc(ESC);
			uart_putc(ESC_END);
			break;
			
		case ESC:
			uart_putc(ESC);
			uart_putc(ESC_ESC);
			break;
			
		default:
			uart_putc(package[p]);
		}
		p++;
	}
	
	uart_putc(END);
}
