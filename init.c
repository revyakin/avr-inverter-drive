/*******************************************************************************
 * $Id: init.c 6 2012-02-03 23:42:58Z reviakinea@gmail.com $
 ******************************************************************************/

#include <avr/io.h>

#include "gpio.h"
#include "mcu/pll/pll_drv.h"

//#define	PSCOUT00	PD0
//#define	PSCOUT01	PB7

//#define	PSCOUT10	PC0
//#define	PSCOUT11	PB6

//#define	PSCOUT20	PB0
//#define	PSCOUT21	PB1
//#define	PSCOUT22	PC2
//#define	PSCOUT23	PC3

void init(void) {
	/*************************************************************************************/
	/*           ports direction configuration                                           */
	/*************************************************************************************/
	
	DDRB = 0;
	DDRC = 0;
	DDRD = 0;
	DDRE = 0;
	
	PORTB = 0;
	PORTC = 0;
	PORTD = 0;
	PORTE = 0;

	/* PWM outputs */
	GPIO_DIRECTION(PSCOUT00, OUTPUT);
	GPIO_DIRECTION(PSCOUT01, OUTPUT);
	GPIO_DIRECTION(PSCOUT10, OUTPUT);
	GPIO_DIRECTION(PSCOUT11, OUTPUT);
	GPIO_DIRECTION(PSCOUT20, OUTPUT);
	GPIO_DIRECTION(PSCOUT21, OUTPUT);

	/* Leds */
	GPIO_DIRECTION(LED,    OUTPUT);
	GPIO_DIRECTION(WD_LED, OUTPUT);

	/* Encoder */
	GPIO_DIRECTION(ENCODER_A, INPUT_PULLUP);
	GPIO_DIRECTION(ENCODER_B, INPUT_PULLUP);
	
	
//	DDRB = 0xC3;	// 11000011
//	DDRC = 0x89;	// 10001001
//	DDRD = 0x01;	// 00000001
//	DDRE = 0x02;	// 00000010

    /*************************************************************************************/
    /*     Timer 0 Configuration : generates the sampling frequency                      */
    /*************************************************************************************/
	TCCR0A = (1<<WGM01);   // mode CTC : Clear Timer on Compare
	TCCR0B = (1<<CS02);    // f_quartz = 12 MHz / 256 = 48 kHz
	OCR0A = 0x2f;          // one interruption every ms
	TIMSK0 = (1<<OCIE0A);  // allow interruption when timer=compare

	/**************************************************************************************/
	/* Timer1: normal mode, ext. clock on rising edge. Used to calculate the motor speed  */
	/**************************************************************************************/
	TCCR1A = 0;
	TCCR1B = _BV(CS10) | _BV(CS11) | _BV(CS12);
	TCCR1C = 0;
	
//	//Start_pll_64_mega();   // start the PLL at 64 MHz
//	//Wait_pll_ready();


	SREG|=0x80;            // allow interruptions,
}
